#include "plugin/cper_artifact.hpp"

#include <fcntl.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <filesystem>
#include <system_error>

namespace phosphor::logging::plugin::cper
{

namespace
{

/**
 * @brief Copy artifact data between file descriptors.
 *
 * @param[in] sourceFd Source file descriptor.
 * @param[in] destinationFd Destination file descriptor.
 */
void copyArtifact(int sourceFd, int destinationFd)
{
    std::array<std::byte, artifactCopyBufferSize> buffer{};

    while (true)
    {
        const auto bytesRead = ::read(sourceFd, buffer.data(), buffer.size());

        if (bytesRead == 0)
        {
            return;
        }

        if (bytesRead < 0)
        {
            throw std::system_error(errno, std::generic_category(),
                                    "Failed to read CPER artifact");
        }

        ssize_t remaining = bytesRead;

        const auto* data = reinterpret_cast<const char*>(buffer.data());

        while (remaining > 0)
        {
            const auto bytesWritten = ::write(destinationFd, data, remaining);

            if (bytesWritten < 0)
            {
                throw std::system_error(errno, std::generic_category(),
                                        "Failed to write CPER artifact");
            }

            remaining -= bytesWritten;
            data += bytesWritten;
        }
    }
}

} // namespace

std::filesystem::path artifactPath(const std::string& objectPath,
                                   const std::filesystem::path& root)
{
    const auto separatorPos = objectPath.find_last_of('/');

    const auto entryId = objectPath.substr(
        separatorPos == std::string::npos ? 0 : separatorPos + 1);

    return root / (entryId + ".bin");
}

std::filesystem::path persistArtifact(const std::string& objectPath, int fd,
                                      const std::filesystem::path& root)
{
    const auto path = artifactPath(objectPath, root);

    std::filesystem::create_directories(path.parent_path());

    const auto outputFd =
        ::open(path.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);

    if (outputFd < 0)
    {
        throw std::system_error(errno, std::generic_category(),
                                "Failed to create CPER artifact");
    }

    try
    {
        /*
         * CPER producers are expected to provide a regular
         * file descriptor. Rewind before persisting.
         */
        if (::lseek(fd, 0, SEEK_SET) < 0)
        {
            throw std::system_error(errno, std::generic_category(),
                                    "Failed to rewind CPER artifact");
        }

        copyArtifact(fd, outputFd);

        if (::close(outputFd) < 0)
        {
            throw std::system_error(errno, std::generic_category(),
                                    "Failed to close CPER artifact");
        }
    }
    catch (...)
    {
        ::close(outputFd);

        std::error_code ec;
        std::filesystem::remove(path, ec);

        throw;
    }

    return path;
}

} // namespace phosphor::logging::plugin::cper

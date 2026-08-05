# phosphor-logging Plugin Framework

## Overview

The phosphor-logging plugin framework allows log entries to be extended with
plugin-specific functionality while keeping the Logging Manager independent of
plugin implementations.

The framework provides a common mechanism for:

- Requesting plugins from event metadata
- Generating plugin descriptors
- Creating runtime plugin instances
- Associating plugins with log entries

This allows new plugin types to be added without introducing plugin-specific
logic into the Logging Manager.

---

## Architecture

The framework separates plugin request processing from runtime plugin creation.

```text
Plugin Request Source
          │
          ▼
      PluginInfo
          │
          ▼
    PluginRegistry
          │
          ▼
 PluginFactory::createDescriptor()
          │
          ▼
      Descriptor
          │
          ▼
    PluginManager
          │
          ▼
 PluginFactory::create()
          │
          ▼
        Plugin
          │
          ▼
         Entry
```

### Framework Flow

1. A plugin request is translated into a `PluginInfo`.
2. `PluginRegistry` resolves the factory associated with the requested plugin
   type.
3. The factory validates the request and generates a `Descriptor`.
4. `PluginManager` uses the descriptor to create a runtime plugin instance.
5. Ownership of the created plugin is transferred to the associated log entry.

---

## Core Concepts

### PluginInfo

`PluginInfo` represents a plugin creation request.

Example:

```text
PluginInfo
{
    type = cper,
    data = {
        ...
    }
}
```

`PluginInfo` objects are short-lived and exist only during request processing.

### Descriptor

A descriptor contains the validated information required to create a runtime
plugin instance.

Descriptors:

- Encapsulate plugin creation data
- Decouple request processing from runtime creation
- Allow validation before plugin instantiation

Descriptors are transient transport objects used between descriptor generation
and runtime plugin creation. They are discarded once the runtime plugin has been
created.

### PluginFactory

Each plugin type provides a factory implementation.

Factories are responsible for:

1. Translating `PluginInfo` into a descriptor
2. Creating runtime plugin instances from descriptors

```text
PluginInfo
    │
    ▼
createDescriptor()
    │
    ▼
Descriptor
    │
    ▼
create()
    │
    ▼
Plugin
```

### PluginRegistry

The plugin registry maintains the mapping between plugin types and their
associated factories.

Responsibilities:

- Factory registration
- Factory lookup
- Descriptor generation support
- Runtime plugin creation support

The registry owns plugin factories but does not own descriptors, runtime
plugins, or log entries.

> **Note:** The registry is owned by the Logging Manager rather than the
> PluginManager because plugin registration and descriptor generation are
> framework-level services. The PluginManager consumes registry services to
> construct runtime plugins, but does not own or manage plugin registrations.

### PluginManager

The plugin manager coordinates runtime plugin creation.

Responsibilities:

- Resolve the factory associated with a descriptor type
- Invoke the selected factory
- Create runtime plugin instances

`PluginManager` is a construction helper only. It does not retain ownership of
created plugin instances.

### Runtime Plugin

A plugin is the runtime representation of a log entry extension.

Plugins:

- Expose plugin-specific D-Bus interfaces
- Maintain plugin-specific runtime state
- Manage plugin-specific resources

Plugins remain associated with their owning log entry for the lifetime of that
entry.

---

## Event Extensions

Plugins may be explicitly requested through event extension metadata.

Example:

```json
{
  "_EXTENSIONS": {
    "xyz.openbmc_project.Logging.Diagnostic.CPER": {
      "NotificationType": "8f87f311-c998-4d9e-a0c4-fb4dfc2c8c14",
      "SectionType": "dc3ea0b0-a144-4797-b95b-53fa242b6e1d",
      "Oem": {
        "AMD": {
          "CperSection": {
            "AID": "0x1234",
            "Socket": 0,
            "Die": 1
          }
        }
      }
    }
  }
}
```

The `Oem` property contains implementation-specific metadata associated with the
CPER record. The structure and contents are vendor-defined and interpreted by
the corresponding plugin implementation.

`_EXTENSIONS` is framework transport metadata used to construct plugin requests.
It is consumed during plugin creation and is not considered part of the runtime
log entry state.

Unsupported extensions are ignored.

---

## Plugin Request Sources

Plugin requests may originate from multiple sources:

- Explicit event extensions (`_EXTENSIONS`)
- Platform/OEM plugin providers
- Future framework-defined request providers

Regardless of the source, plugin creation follows a common framework path:

```text
Plugin Request Source
          │
          ▼
      PluginInfo
          │
          ▼
      Descriptor
          │
          ▼
        Plugin
```

This ensures all plugins are created consistently through the framework while
keeping the Logging Manager independent of plugin-specific policy and
implementation details.

### Platform/OEM Plugin Providers

While some plugins are explicitly requested through event extensions, others may
be derived from log metadata using platform-specific policy.

Example:

```text
AdditionalData
      │
      ▼
Platform/OEM Policy
      │
      ▼
    PluginInfo
```

Platform/OEM providers generate `PluginInfo` objects only. Descriptor generation
and runtime plugin creation remain framework responsibilities.

### Design Principles

- Entry metadata remains independent of plugin metadata
- `_EXTENSIONS` is framework transport metadata
- Plugin providers generate `PluginInfo` objects
- Descriptor generation remains a framework responsibility
- Runtime plugins own plugin-specific state
- The Logging Manager remains plugin-agnostic

The resulting flow is:

```text
_EXTENSIONS ──────┐
                  │
OEM Providers ────┤
                  ▼
             PluginInfo
                  ▼
             Descriptor
                  ▼
               Plugin
                  ▼
                Entry
```

---

## Ownership Model

The framework separates plugin registration, plugin construction, and runtime
ownership.

```text
Logging Manager
 │
 ├── PluginRegistry
 │     └── Registered Factories
 │
 ├── PluginManager
 │
 └── Entry
       │
       ├── Entry State
       │
       └── Plugin(s)
             │
             └── Plugin-specific State
```

Ownership summary:

- The `Logging Manager` owns the `PluginRegistry`
- The `Logging Manager` owns the `PluginManager`
- `PluginRegistry` owns registered plugin factories
- `PluginManager` coordinates plugin construction but does not retain ownership
  of created plugins
- `Entry` owns runtime plugin instances
- Plugins own plugin-specific interfaces, metadata, persisted artifacts, and
  runtime state
- `Descriptor` objects are transient transport objects used only during plugin
  creation

The framework intentionally separates:

```text
PluginRegistry  -> Factory registration and lookup
PluginManager   -> Plugin construction
Plugin          -> Runtime functionality
Entry           -> Runtime ownership
```

This separation keeps the Logging Manager independent of plugin implementations
while allowing each plugin to manage its own interfaces, metadata, persistence,
restore, and deletion behavior.

---

## Plugin Resource Lifecycle

Plugins are responsible for managing any plugin-specific resources associated
with their runtime state.

Typical lifecycle:

```text
Create
  ↓
Persist
  ↓
Restore
  ↓
Delete
```

Examples of plugin-managed resources include:

- Persisted artifacts
- Plugin-specific metadata
- Auxiliary runtime state

The Logging Manager remains plugin-agnostic and delegates plugin-specific
resource management to the corresponding plugin implementation.

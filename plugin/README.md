# phosphor-logging Plugin Framework

## Overview

The phosphor-logging Plugin Framework allows log entries to be extended with
plugin-specific functionality while keeping the Logging Manager independent of
plugin implementations.

The framework provides a common mechanism for:

1. Processing plugin requests
2. Generating plugin descriptors
3. Creating runtime plugin instances
4. Associating plugins with log entries

Plugin requests are represented by `PluginRequest` objects and may originate
from:

- Event extension metadata (`_EXTENSIONS`)
- Platform-specific hooks
- OEM-specific hooks
- Future native event support

All plugin requests follow the same flow:

```text
PluginRequest
      │
      ▼
  Descriptor
      │
      ▼
    Plugin
      │
      ▼
   Log Entry
```

This allows new plugin types and plugin request sources to be added without
introducing plugin-specific logic into the Logging Manager.

---

## Architecture

The framework separates plugin request processing from runtime plugin creation.

```text
PluginRequest
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
 Runtime Plugin
      │
      ▼
   Log Entry
```

---

## Core Components

### PluginRequest

`PluginRequest` represents a request to associate a plugin with a log entry.

```cpp
PluginRequest
{
    interface =
        "xyz.openbmc_project.Logging.Diagnostic.CPER",

    data = {
        ...
    }
}
```

`PluginRequest` objects are transient and exist only during request processing.

Requests may originate from event metadata, platform hooks, OEM hooks, or future
event-creation mechanisms.

---

### Descriptor

A descriptor contains validated plugin-specific data required to construct a
runtime plugin instance.

Descriptors:

- Encapsulate plugin creation data
- Separate request processing from runtime creation
- Allow validation before instantiation

Descriptors are transient transport objects used only during plugin creation.

---

### PluginFactory

Each plugin provides a factory responsible for:

1. Translating a `PluginRequest` into a descriptor
2. Creating runtime plugin instances from descriptors

```text
PluginRequest
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

This allows request interpretation and runtime construction to remain
plugin-specific.

---

### PluginRegistry

The registry maintains the mapping between extension interfaces and their
associated factories.

Responsibilities:

- Factory registration
- Factory lookup
- Descriptor generation support
- Runtime plugin creation support

The registry owns registered factories but does not own descriptors, plugins, or
log entries.

---

### PluginManager

The plugin manager creates runtime plugins from descriptors.

Responsibilities:

- Resolve the factory associated with a descriptor
- Invoke the selected factory
- Create runtime plugin instances

`PluginManager` is responsible only for plugin construction and lifecycle
integration.

---

### Runtime Plugin

A plugin is the runtime representation of an event extension associated with a
log entry.

Plugins may:

- Expose plugin-specific D-Bus interfaces
- Maintain runtime state
- Manage plugin-specific resources
- Participate in log-entry lifecycle management

Plugins remain associated with their owning log entry for the lifetime of that
entry.

---

## Plugin Request Sources

Plugin requests may originate from multiple sources and are normalized into a
common representation before plugin creation.

```text
                  Event
                    │
     ┌──────────────┼──────────────┐
     │              │              │
     ▼              ▼              ▼

 _EXTENSIONS    OEM Hook    Platform Hook

     │              │              │
     └──────────────┴──────────────┘
                    │
                    ▼
              PluginRequest
                    │
                    ▼
                Descriptor
                    │
                    ▼
                  Plugin
                    │
                    ▼
                 Log Entry
```

This allows new request mechanisms to be added without modifying plugin
construction, ownership, lifetime management, or runtime behavior.

---

## Ownership Model

```text
Logging Manager
      │
      ├── PluginRegistry
      │
      ├── PluginManager
      │
      └── Entry
            │
            └── Plugin(s)
```

Ownership rules:

- Logging Manager owns `PluginRegistry`
- Logging Manager owns `PluginManager`
- PluginRegistry owns registered factories
- Entry owns runtime plugin instances
- Descriptor objects are transient and discarded after plugin creation

The framework intentionally separates responsibilities:

```text
PluginRegistry  -> Factory registration and lookup
PluginManager   -> Plugin construction
Plugin          -> Runtime functionality
Entry           -> Runtime ownership
```

---

## Event Extensions

Plugins may be requested through event extension metadata.

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

The `_EXTENSIONS` property acts as a transport mechanism for extension-specific
metadata.

During log creation, supported extensions are translated into one or more
`PluginRequest` objects and processed through the standard plugin creation
pipeline.

```text
_EXTENSIONS
      │
      ▼
PluginRequest
      │
      ▼
 Descriptor
      │
      ▼
   Plugin
      │
      ▼
 Log Entry
```

The Logging Manager remains independent of extension-specific schemas and
delegates interpretation of extension metadata to the corresponding plugin
implementation.

Unsupported extensions are ignored.

---

## Hook-Based Plugin Requests

In addition to `_EXTENSIONS` metadata, platforms may register hooks that inspect
event data and contribute additional plugin requests.

```text
Event Created
      │
      ▼
Platform Hook
      │
      ▼
PluginRequest
      │
      ▼
 Descriptor
      │
      ▼
   Plugin
```

This enables platform-specific or OEM-specific functionality without requiring
modifications to event metadata schemas.

All request sources ultimately converge on the same descriptor and plugin
creation pipeline.

---

## Design Principles

### Logging Manager Independence

The Logging Manager is never aware of plugin-specific schemas, D-Bus interfaces,
or implementation details.

Its responsibilities are limited to:

- Collecting plugin requests
- Coordinating descriptor generation
- Constructing runtime plugins
- Managing plugin ownership through log entries

### Extensibility

Adding support for a new plugin requires:

1. Defining a plugin implementation
2. Providing a descriptor type
3. Registering a factory

No changes are required in:

- Logging Manager
- PluginManager
- PluginRegistry interfaces
- Existing plugin implementations

### Source Independence

Plugin requests may originate from:

- `_EXTENSIONS` metadata
- Platform hooks
- OEM hooks
- Future event-generation mechanisms

All sources are normalized into a common `PluginRequest` representation before
entering the plugin pipeline.

---

## Terminology

| Term            | Description                                                      |
| --------------- | ---------------------------------------------------------------- |
| Event Extension | User-visible event metadata attached to an event                 |
| `_EXTENSIONS`   | Transport mechanism used by events to request extensions         |
| PluginRequest   | Internal representation of a request for plugin creation         |
| Descriptor      | Validated plugin creation data                                   |
| PluginFactory   | Creates descriptors and runtime plugins                          |
| PluginRegistry  | Maintains the mapping between extension interfaces and factories |
| PluginManager   | Coordinates runtime plugin construction                          |
| Plugin          | Runtime implementation associated with a log entry               |

This terminology intentionally separates the **Event Extension** concept used by
the event-logging design from the **Plugin Framework** used to implement
extension-specific functionality within phosphor-logging.

# AMD Event Log Extension

## Overview

This extension provides AMD-specific functionality for the phosphor-logging
framework.

It builds on the extension infrastructure provided by phosphor-logging and
enables AMD-specific processing during log creation.

The extension framework allows AMD-specific logic to enrich log entries without
modifying the core logging implementation.

The AMD Event Log (AEL) extension uses a build-time generated reverse lookup
table (LUT) to resolve AMD-specific event metadata from OpenBMC event messages
and associated AdditionalData.

Resolved metadata is exported through the AMD runtime metadata framework and can
optionally be projected into an AMD OEM Redfish payload.

## AMD Metadata Namespace

AMD-specific metadata is stored under the `AMD` namespace within the runtime
metadata object managed by phosphor-logging.

### Example

```json
{
  "AMD": {
    "Key": "Value"
  }
}
```

### Guidelines

- Only the `AMD` namespace is owned and modified by AMD extensions.
- Metadata should be structured and compact.
- Existing fields should remain backward compatible.
- Consumers should ignore unrecognized fields.

---

## Runtime Metadata

The AMD extension framework supports enriching log entries with AMD-specific
runtime metadata.

Metadata is generated during log creation and stored under the `AMD` namespace
as structured JSON.

The AEL implementation derives metadata from:

- Event message identifiers
- Event AdditionalData
- Generated AFID lookup tables
- Static platform metadata

This design eliminates runtime JSON parsing and provides deterministic lookup
behavior.

---

## AEL Metadata

The AMD Event Log (AEL) extension contributes AMD-specific event information
derived from the event context and generated AFID database.

### AEL Core Fields

| Field                    | Description                                     |
| ------------------------ | ----------------------------------------------- |
| `AEL.VERSION`            | Version of the AEL schema                       |
| `AEL.SCHEMA`             | AMD Redfish schema identifier                   |
| `AEL.AFID`               | AMD Field Identifier identifying the event      |
| `AEL.DESCRIPTION`        | Human-readable description associated with AFID |
| `AEL.FRU_LIST`           | Inventory object path(s) associated with fault  |
| `AEL.RACK_UNIT_POSITION` | Rack unit position associated with the platform |
| `AEL.REDFISH`            | Optional pre-rendered AMD OEM Redfish payload   |

### Example

```json
{
  "AMD": {
    "AEL.VERSION": "1.0",
    "AEL.SCHEMA": "#AMD_Message.v1_0_0.AMD_Message",
    "AEL.AFID": 12001,
    "AEL.DESCRIPTION": "Compute Tray Error",
    "AEL.FRU_LIST": "/redfish/v1/Chassis/chassis0",
    "AEL.RACK_UNIT_POSITION": "Rack-1-U10"
  }
}
```

---

## AFID Reverse Lookup Table

The AEL extension uses a build-time generated reverse lookup table to resolve
AMD-specific metadata.

The lookup table is defined in JSON and transformed into native C++ during the
build process.

### Features

- Provides mapping of OpenBMC events to AMD-specific metadata.
- Uses a JSON-defined reverse LUT generated at build time.
- Eliminates runtime JSON parsing through generated C++ code.
- Ensures deterministic and efficient lookup behavior.
- Supports OEM customization through Yocto-based JSON override.

### Supported Metadata

Each lookup entry may provide:

- AFID
- Description
- OriginOfCondition
- Redfish mappings

### Lookup Flow

```text
Event Message
      |
      v
AdditionalData
      |
      v
Generated AFID Lookup Database
      |
      v
AFID Metadata
      |
      v
AMD Runtime Metadata
      |
      +------> Redfish Projection
```

---

## Build-Time Code Generation

The reverse lookup infrastructure is generated during the build process from a
JSON configuration.

### Generated Artifacts

```text
amd_afid_db_gen.hpp
```

### Generated APIs

```cpp
AFIDInfo lookupAFID(
    const std::string& message,
    const std::map<std::string, std::string>& additionalData);

AFIDStaticInfo getStaticData();
```

### Static Metadata

The generated database exports platform metadata including:

- Schema version
- Redfish schema identifier
- Rack unit position
- Fallthrough AFID

This approach preserves a JSON-driven workflow while eliminating runtime parsing
and lookup overhead.

---

## Runtime AFID Resolution

During log creation, the AEL runtime integration layer performs the following
operations:

1. Receives the event message identifier.
2. Receives the event AdditionalData.
3. Resolves AFID metadata using the generated lookup database.
4. Retrieves static platform metadata.
5. Constructs AMD runtime metadata.
6. Projects metadata into an AMD OEM Redfish payload when required.

The runtime implementation is intentionally separated from the generated lookup
infrastructure, allowing lookup data and runtime behavior to evolve
independently.

---

## Redfish Projection

The AEL framework supports projecting AMD runtime metadata into an AMD OEM
Redfish representation.

By default, the framework generates a Redfish payload from available AEL
metadata.

Applications may optionally provide a fully rendered OEM Redfish payload using
the `AEL.REDFISH` field.

When present, the supplied payload is treated as the authoritative AMD OEM
payload and automatic projection is skipped.

### Supported Projection Modes

#### 1. Metadata-Based Projection

The framework automatically generates an AMD OEM Redfish payload using resolved
AFID metadata.

##### Input

```json
{
  "AMD": {
    "AEL.AFID": 12001,
    "AEL.DESCRIPTION": "Compute Tray Error",
    "AEL.FRU_LIST": "/redfish/v1/Chassis/chassis0"
  }
}
```

##### Projected Output

```json
{
  "@odata.type": "#AMD_Message.v1_0_0.AMD_Message",
  "AMDFieldIdentifiers": [
    {
      "AFID": 12001,
      "Description": "Compute Tray Error",
      "ServiceableUnits": [
        {
          "@odata.id": "/redfish/v1/Chassis/chassis0"
        }
      ]
    }
  ],
  "AMDFieldIdentifiers@odata.count": 1
}
```

#### 2. Pre-rendered OEM Payload (Passthrough)

Applications may provide a fully rendered AMD OEM Redfish payload.

##### Input

```json
{
  "AMD": {
    "AEL.REDFISH": {
      "@odata.type": "#AMD_Message.v1_0_0.AMD_Message",
      "AMDFieldIdentifiers": [
        {
          "AFID": 12001,
          "Description": "Compute Tray Error"
        }
      ]
    }
  }
}
```

##### Output

```json
{
  "@odata.type": "#AMD_Message.v1_0_0.AMD_Message",
  "AMDFieldIdentifiers": [
    {
      "AFID": 12001,
      "Description": "Compute Tray Error"
    }
  ]
}
```

When `AEL.REDFISH` is present, the payload is used directly and no additional
metadata processing or projection is performed.

---

## Extensibility

The AMD extension infrastructure is designed to support future AMD-specific
logging enhancements using the same runtime metadata framework.

Future features may include:

- Additional AFID metadata fields
- Enhanced Redfish projections
- Platform-specific OEM extensions
- Diagnostics integration
- Serviceability enhancements

Extensions may contribute new metadata, diagnostics, or log enrichment
capabilities under the `AMD` namespace while maintaining a consistent
programming model and preserving compatibility with existing consumers.

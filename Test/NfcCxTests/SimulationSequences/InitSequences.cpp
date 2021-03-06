//
// Copyright (c) Microsoft Corporation.  All Rights Reserved
//

#include "Precomp.h"

#include <phNciNfc.h>
#include <phNciNfc_Core.h>
#include <phNciNfc_CoreStatus.h>
#include <phNciNfc_RfConfig.h>

#include "InitSequences.h"
#include "SEInitializationSequences.h"

const SimSequenceStep InitSequences::Reset::NciResetCommand = SimSequenceStep::NciControlWrite(
    L"CORE_RESET_CMD",
    {
        // NFC Controller Interface (NCI), Version 1.1, Section 4.1, CORE_RESET_CMD
        phNciNfc_e_NciCoreMsgTypeCntrlCmd,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreResetCmdOid,
        {
            phNciNfc_ResetType_KeepConfig, // Reset type
        },
    }
);

const SimSequenceStep InitSequences::Reset::NciResetResponse_Nci1 = SimSequenceStep::NciControlRead(
    L"CORE_RESET_RSP",
    {
        // NFC Controller Interface (NCI), Version 1.1, Section 4.1, CORE_RESET_RSP
        phNciNfc_e_NciCoreMsgTypeCntrlRsp,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreResetCmdOid,
        {
            PH_NCINFC_STATUS_OK, // Status
            0x11, // NCI version (1.1)
            phNciNfc_ResetType_KeepConfig, // Reset type
        },
    }
);

const SimSequenceStep InitSequences::Reset::NciResetResponse_Nci2 = SimSequenceStep::NciControlRead(
    L"CORE_RESET_RSP",
    {
        // NFC Controller Interface (NCI), Version 2.0, Section 4.1, CORE_RESET_RSP
        phNciNfc_e_NciCoreMsgTypeCntrlRsp,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreResetCmdOid,
        {
            PH_NCINFC_STATUS_OK, // Status
        },
    }
);

const SimSequenceStep InitSequences::Reset::NciResetNotification_Nci2 = SimSequenceStep::NciControlRead(
    L"CORE_RESET_NTF",
    {
        // NFC Controller Interface (NCI), Version 2.0, Section 4.1, CORE_RESET_NTF
        phNciNfc_e_NciCoreMsgTypeCntrlNtf,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreResetCmdOid,
        {
            0x02, // Reset trigger
            0x00, // Configuration status (configuration kept)
            0x20, // Nci version
            0x00, // Manufacturer ID
            0x00, // Manufacturer info length
        },
    }
);

const SimSequenceStep InitSequences::Initialize::PreInitialize = SimSequenceStep::SequenceHandler(
    L"SequencePreInit",
    SequencePreInit,
    STATUS_SUCCESS,
    NFC_CX_SEQUENCE_PRE_INIT_FLAG_SKIP_CONFIG
);

const SimSequenceStep InitSequences::Initialize::InitializeCommand_Nci1 = SimSequenceStep::NciControlWrite(
    L"CORE_INIT_CMD",
    {
        // NFC Controller Interface (NCI), Version 1.1, Section 4.2, CORE_INIT_CMD
        phNciNfc_e_NciCoreMsgTypeCntrlCmd,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreInitCmdOid,
    }
);

const SimSequenceStep InitSequences::Initialize::InitializeCommand_Nci2 = SimSequenceStep::NciControlWrite(
    L"CORE_INIT_CMD",
    {
        // NFC Controller Interface (NCI), Version 2.0, Section 4.2, CORE_INIT_CMD
        phNciNfc_e_NciCoreMsgTypeCntrlCmd,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreInitCmdOid,
        {
            0x00, 0x00, // Feature enable (disable all post NCI 2.0 features)
        }
    }
);

const SimSequenceStep InitSequences::Initialize::InitializeResponse_Nci1 = SimSequenceStep::NciControlRead(
    L"CORE_INIT_RSP",
    {
        // NFC Controller Interface (NCI), Version 1.1, Section 4.2, CORE_INIT_RSP
        phNciNfc_e_NciCoreMsgTypeCntrlRsp,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreInitCmdOid,
        {
            PH_NCINFC_STATUS_OK, // Status
            0b00000011, // Supported discovery config (Merges RF config, Frequency config supported)
            0b00011110, // Supported routing types (Technology, Protocol, AID, NFCID2)
            0b00000011, // Supported routing power modes (Switched off, Battery off)
            0b00000000, // Proprietary capabilities
            5, // Number of supported interfaces, that follow.
                phNciNfc_e_RfInterfacesNfceeDirect_RF,
                phNciNfc_e_RfInterfacesFrame_RF,
                phNciNfc_e_RfInterfacesISODEP_RF,
                phNciNfc_e_RfInterfacesNFCDEP_RF,
                phNciNfc_e_RfInterfacesTagCmd_RF,
            1, // Max logical channels
            0xFF, 0xFF, // Max routing table size
            0xFF, // Max control packet payload size
            0xFF, 0xFF, // Max size for large parameters
            0x0, // Manufacturer ID
            0x0, 0x0, 0x0, 0x0, // Manufacturer specific info
        },
    }
);

const SimSequenceStep InitSequences::Initialize::InitializeResponse_Nci2 = SimSequenceStep::NciControlRead(
    L"CORE_INIT_RSP",
    {
        // NFC Controller Interface (NCI), Version 2.0, Section 4.2, CORE_INIT_RSP
        phNciNfc_e_NciCoreMsgTypeCntrlRsp,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreInitCmdOid,
        {
            PH_NCINFC_STATUS_OK,
            0b00001011, // Supported discovery config (HCI network, Merges RF config, Frequency config supported)
            0b00011110, // Supported routing types (Technology, Protocol, AID, System code)
            0b00000011, // Supported routing power modes (Switched off, Battery off)
            0b00000000, // Proprietary capabilities
            0, // Max number of Dynamic Logical Connections
            0xFF, 0xFF, // Max routing table size
            0xFF, // Max control packet payload size
            0xFF, // Max static HCI packet size
            1, // Number of static HCI network connection credits
            0xFF, 0xFF, // Max NFC-V RF frame size
            5, // Number of supported interfaces, that follow.
                phNciNfc_e_RfInterfacesNfceeDirect_RF, // Interface
                0, // Number of extensions

                phNciNfc_e_RfInterfacesFrame_RF, // Interface
                0, // Number of extensions

                phNciNfc_e_RfInterfacesISODEP_RF, // Interface
                0, // Number of extensions

                phNciNfc_e_RfInterfacesNFCDEP_RF, // Interface
                0, // Number of extensions

                phNciNfc_e_RfInterfacesTagCmd_RF, // Interface
                0, // Number of extensions
        }
    }
);

const SimSequenceStep InitSequences::Initialize::InitializeComplete = SimSequenceStep::SequenceHandler(
    L"SequenceInitComplete",
    SequenceInitComplete,
    STATUS_SUCCESS,
    0
);

const SimSequenceStep InitSequences::Initialize::GetConfigCommand = SimSequenceStep::NciControlWrite(
    L"CORE_GET_CONFIG_CMD",
    {
        // NFC Controller Interface (NCI), Version 1.1, Section 4.3.2, CORE_GET_CONFIG_CMD
        phNciNfc_e_NciCoreMsgTypeCntrlCmd,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreGetConfigCmdOid,
        {
            0x4, // Number of parameters, that follow.
                PHNCINFC_RFCONFIG_ATR_REQ_GEN_BYTES,
                PHNCINFC_RFCONFIG_ATR_REQ_CONFIG,
                PHNCINFC_RFCONFIG_ATR_RES_GEN_BYTES,
                PHNCINFC_RFCONFIG_ATR_RES_CONFIG,
        },
    }
);

const SimSequenceStep InitSequences::Initialize::GetConfigResponse = SimSequenceStep::NciControlRead(
    L"CORE_GET_CONFIG_RSP",
    {
        // NFC Controller Interface (NCI), Version 1.1, Section 4.3.2, CORE_GET_CONFIG_RSP
        phNciNfc_e_NciCoreMsgTypeCntrlRsp,
        phNciNfc_e_CoreNciCoreGid,
        phNciNfc_e_NciCoreGetConfigCmdOid,
        {
            PH_NCINFC_STATUS_OK, // Status
            0x4, // Number of parameters, that follow.
                // Simple TLV format: { 1 byte tag, 1 byte length, <256 bytes payload }
                PHNCINFC_RFCONFIG_ATR_REQ_GEN_BYTES, // Id
                0x11, // Length
                0x46, 0x66, 0x6D, 0x01, 0x01, 0x11, 0x02, 0x02, 0x03, 0x80, 0x03, 0x02, 0x00, 0x01, 0x04, 0x01, 0x64, // Value

                PHNCINFC_RFCONFIG_ATR_REQ_CONFIG, // Id
                0x01, // Length
                0x30, // Value

                PHNCINFC_RFCONFIG_ATR_RES_GEN_BYTES, // Id
                0x11, // Length
                0x46, 0x66, 0x6D, 0x01, 0x01, 0x11, 0x02, 0x02, 0x03, 0x80, 0x03, 0x02, 0x00, 0x01, 0x04, 0x01, 0x64, // Value

                PHNCINFC_RFCONFIG_ATR_RES_CONFIG, // Id
                0x01, // Length
                0x30, // Value
        },
    }
);

const SimSequenceStep InitSequences::Uninitialize::PreShutdown = SimSequenceStep::SequenceHandler(
    L"SequencePreShutdown",
    SequencePreShutdown,
    STATUS_SUCCESS,
    0
);

const SimSequenceStep InitSequences::Uninitialize::ShutdownComplete = SimSequenceStep::SequenceHandler(
    L"SequenceShutdownComplete",
    SequenceShutdownComplete,
    STATUS_SUCCESS,
    0
);

const SimSequenceStep InitSequences::Power::D0Entry = SimSequenceStep::D0Entry(
    L"D0Entry"
);

const SimSequenceStep InitSequences::Power::D0Exit = SimSequenceStep::D0Exit(
    L"D0Exit"
);

const SimSequenceStep InitSequences::Reset::Sequence_Nci1[2] =
{
    NciResetCommand,
    NciResetResponse_Nci1,
};

const SimSequenceStep InitSequences::Reset::Sequence_Nci2[3] =
{
    NciResetCommand,
    NciResetResponse_Nci2,
    NciResetNotification_Nci2,
};

// Error-free initialize that reports 0 attached SEs using NCI 1.1.
const SimSequenceView InitSequences::Initialize::NoSEsSequence_Nci1[8] =
{
    PreInitialize,
    Reset::Sequence_Nci1,
    InitializeCommand_Nci1,
    InitializeResponse_Nci1,
    InitializeComplete,
    GetConfigCommand,
    GetConfigResponse,
    SEInitializationSequences::NoSEs::InitializeSequence_Nci1,
};

// Error-free initialize that reports 0 attached SEs using NCI 2.0.
const SimSequenceView InitSequences::Initialize::NoSEsSequence_Nci2[8] =
{
    PreInitialize,
    Reset::Sequence_Nci2,
    InitializeCommand_Nci2,
    InitializeResponse_Nci2,
    InitializeComplete,
    GetConfigCommand,
    GetConfigResponse,
    SEInitializationSequences::NoSEs::InitializeSequence_Nci2,
};

const SimSequenceView
InitSequences::Initialize::NoSEsSequence(bool isNci2)
{
    if (isNci2)
    {
        return NoSEsSequence_Nci2;
    }

    return NoSEsSequence_Nci1;
}

// Error-free initialize with an eSE using NCI 1.1.
const SimSequenceView InitSequences::Initialize::WithEseSequence_Nci1[8] =
{
    PreInitialize,
    Reset::Sequence_Nci1,
    InitializeCommand_Nci1,
    InitializeResponse_Nci1,
    InitializeComplete,
    GetConfigCommand,
    GetConfigResponse,
    SEInitializationSequences::WithEse::InitializeSequence_Nci1,
};

// Error-free initialize with an eSE using NCI 2.0.
const SimSequenceView InitSequences::Initialize::WithEseSequence_Nci2[8] =
{
    PreInitialize,
    Reset::Sequence_Nci2,
    InitializeCommand_Nci2,
    InitializeResponse_Nci2,
    InitializeComplete,
    GetConfigCommand,
    GetConfigResponse,
    SEInitializationSequences::WithEse::InitializeSequence_Nci2,
};

const SimSequenceView
InitSequences::Initialize::WithEseSequence(bool isNci2)
{
    if (isNci2)
    {
        return WithEseSequence_Nci2;
    }

    return WithEseSequence_Nci1;
}

// Error-free NCI 1.1 uninitialize.
const SimSequenceView InitSequences::Uninitialize::Sequence_Nci1[3] =
{
    PreShutdown,
    Reset::Sequence_Nci1,
    ShutdownComplete,
};

// Error-free NCI 2.0 uninitialize.
const SimSequenceView InitSequences::Uninitialize::Sequence_Nci2[3] =
{
    PreShutdown,
    Reset::Sequence_Nci2,
    ShutdownComplete,
};

const SimSequenceView
InitSequences::Uninitialize::Sequence(bool isNci2)
{
    if (isNci2)
    {
        return Sequence_Nci2;
    }

    return Sequence_Nci1;
}

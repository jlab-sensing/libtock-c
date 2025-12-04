#!/usr/bin/env python3
# pip install pycryptodome

from Crypto.Cipher import AES
import binascii

# ==============================
# Configuration (from your device)
# ==============================

# AppKey / NwkKey (16 bytes)
NWK_KEY = bytes.fromhex("B82D913FAFB099EE2E3BBE9B3196C109")

# JoinAccept payload received from server (without MIC, 12-17 bytes)
# Example from your log: 20 8f 00 00 13 00 00 24 f3 0c 26 88 05 00 ff 00 00 00 00 00 02 00 00 00 00 00 00 01 fd b5 a0
# The JoinAccept payload in LoRaWAN is the encrypted bytes. We'll assume you have it as hex:
JOIN_ACCEPT_ENC = bytes.fromhex(
    "208f000013000024f30c26880500ff0000000002000000000001fdb5a0"
)

# DevNonce sent in JoinRequest (2 bytes, little endian)
DEV_NONCE = bytes.fromhex("0000")  # example: 0x0000

# ==============================
# LoRaWAN 1.1 MIC calculation
# ==============================

def calculate_joinaccept_mic(join_accept_enc, nwk_key, dev_nonce):
    """
    Calculate the MIC for LoRaWAN JoinAccept (1.1)
    """
    from Crypto.Hash import CMAC

    # In LoRaWAN 1.1, the MIC key is the NwkKey (sNwkSIntKey)
    cobj = CMAC.new(nwk_key, ciphermod=AES)
    
    # Message for MIC: JoinAccept | DevNonce
    # Note: JoinAccept is decrypted bytes if you have them, or raw encrypted bytes
    msg = join_accept_enc + dev_nonce
    
    cobj.update(msg)
    mic = cobj.digest()[:4]  # MIC is 4 bytes
    return mic

mic = calculate_joinaccept_mic(JOIN_ACCEPT_ENC, NWK_KEY, DEV_NONCE)
print("Calculated MIC:", binascii.hexlify(mic).decode())


#ifndef __IKE_PROTOCOL_H__
#define __IKE_PROTOCOL_H__

#include <stdint.h>

#define IKE_HEADER_LENGTH 28
#define IKE_PAYLOAD_HEADER_LENGTH 4

#define IKE_PROPOSAL_LAST		0
#define IKE_PROPOSAL_MORE		2
#define IKE_TRANSFORM_LAST	0
#define IKE_TRANSFORM_MORE	3

typedef enum {
  PLD_NO = 0,
  PLD_SA = 33,
  PLD_KE = 34,
  PLD_IDi = 35,
  PLD_IDr = 36,
  PLD_CERT = 37,
  PLD_CERTREQ = 38,
  PLD_AUTH = 39,
  PLD_Nx = 40,
  PLD_N = 41,
  PLD_D = 42,
  PLD_V = 43,
  PLD_TSi = 44,
  PLD_TSr = 45,
  PLD_SK = 46,
  PLD_CP = 47,
  PLD_EAP = 48
}ike_payload_type;

typedef enum {
  IKE_SA_INIT = 34,
  IKE_AUTH = 35,
  CREATE_CHILD_SA = 36,
  INFORMATIONAL = 37
}ike_exchange_type;

typedef enum {
  PROTOCOL_IKE  = 1,
  PROTOCOL_AH   = 2,
  PROTOCOL_ESP  = 3,
}ike_protocol_id;

typedef enum {
  IKE_ENCR  = 1,
  IKE_PRF   = 2,
  IKE_INTEG = 3,
  IKE_DH    = 4,
  IKE_ESN   = 5,
}ike_transform_type;

typedef enum {
  ENCR_DES_IV64   = 1,
  ENCR_DES        = 2,
  ENCR_3DES       = 3,
  ENCR_RCS        = 4,
  ENCR_IDEA       = 5,
  ENCR_CAST       = 6,
  ENCR_BLOWFISH   = 7,
  ENCR_3IDEA      = 8,
  ENCR_DES_IV32   = 9,
  ENCR_NULL       = 11,
  ENCR_AES_CBC    = 12,
  ENCR_AES_CTR    = 13,
}ike_encr_id;

typedef enum {
  PRF_HMAC_MD5    = 1,
  PRF_HMAC_SHA1   = 2,
  PRF_HMAC_TIGER  = 3,
}ike_prf_id;

typedef enum {
  AUTH_NONE         = 0,
  AUTH_HMAC_MD5_96  = 1,
  AUTH_HMAC_SHA1_96 = 2,
  AUTH_DES_MAC      = 3,
  AUTH_KPDK_MD5     = 4,
  AUTH_AES_XCBC_96  = 5,
}ike_integ_id;

typedef enum {
  MODP_NONE   = 0,
  MODP_768    = 1,
  MODP_1024   = 2,
  MODP_1536   = 5,
  MODP_2048   = 14,
  MODP_3072   = 15,
  MODP_4096   = 16,
  MODP_6144   = 17,
  MODP_8192   = 18,
}ike_dh_id;

typedef enum {
  ESN_0     = 0,
  ESN_1     = 1,
}ike_esn_id;

typedef struct {
  uint64_t  SPIi;
  uint64_t  SPIr;
  uint8_t   next_payload;
  uint8_t   version;
  uint8_t   exchange_type;
  uint8_t   flags;
  uint32_t  message_id;
  uint32_t  length;
}ike_header_t;

typedef struct {
  uint8_t   next_payload;
  uint8_t   reserved;
  uint16_t  length;
}ike_payload_header_t;

#define IKE_PAYLOAD_Nx_FIXED_LENGTH (IKE_PAYLOAD_HEADER_LENGTH)
typedef struct {
  void*     data;
}ike_payload_nx_t;

#define IKE_PAYLOAD_KE_FIXED_LENGTH (IKE_PAYLOAD_HEADER_LENGTH+4)
typedef struct {
  uint16_t  dh_group;
  uint16_t  reserved;
  void*     data;
}ike_payload_ke_t;

#define IKE_PAYLOAD_SA_FIXED_LENGTH (IKE_PAYLOAD_HEADER_LENGTH)
typedef struct {
  void*   proposals;
}ike_payload_sa_t;

#define IKE_PROPOSAL_FIXED_LENGTH 8
typedef struct {
  uint8_t   last;   // 0 is last, 2 is more
  uint8_t   reserved;
  uint16_t  length;
  uint8_t   number;
  uint8_t   protocol;
  uint8_t   spi_size;
  uint8_t   num_of_transforms;
  void*     spi;
  void*			transforms;

	void*			next;
}ike_proposal_t;

#define IKE_TRANSFORM_FIXED_LENGTH 8
typedef struct {
  uint8_t   last;   // 0 is last, 3 is more
  uint8_t   reserved1;
  uint16_t  length;
  uint8_t   type;
  uint8_t   reserved2;
  uint16_t  id;
  void*			attributes;
}ike_transform_t;

typedef struct {
  uint8_t   format;
  uint16_t  type;
  uint16_t  length;
  uint32_t  value;
}ike_attribute_t;

#endif //__IKE_PROTOCOL_H__

#ifndef __IKE_PROTOCOL_H__
#define __IKE_PROTOCOL_H__

#include <stdint.h>

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

typedef struct {
  void*     data;
}ike_payload_nx_t;

typedef struct {
  uint16_t  dh_group;
  uint16_t  reserved;
  void*     data;
}ike_payload_ke_t;

typedef struct {
  uint8_t   format;
  uint16_t  type;
  uint16_t  length;
  uint32_t  value;
}ike_attribute_t;

typedef struct {
  uint8_t   last;   // 0 is last, 3 is more
  uint8_t   reserved1;
  uint16_t  length;
  uint8_t   type;
  uint8_t   reserved2;
  uint16_t  id;
  ike_attribute_t*  attributes;
}ike_transform_t;

typedef struct {
  uint8_t   last;   // 0 is last, 2 is more
  uint8_t   reserved;
  uint16_t  length;
  uint8_t   number;
  uint8_t   protocol;
  uint8_t   spi_size;
  uint8_t   num_of_transforms;
  void*     spi;
  ike_transform_t*  transforms;
}ike_proposal_t;

typedef struct {
  ike_proposal_t*   proposals;
}ike_payload_sa_t;


#endif //__IKE_PROTOCOL_H__

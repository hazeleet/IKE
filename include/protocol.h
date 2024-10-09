#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

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
}payload_type;

typedef enum {
  IKE_SA_INIT = 34,
  IKE_AUTH = 35,
  CREATE_CHILD_SA = 36,
  INFORMATIONAL = 37
}exchange_type;

#endif //__PROTOCOL_H__

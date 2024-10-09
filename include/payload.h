#ifndef __PAYLOAD_H__
#define __PAYLOAD_H__

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
}PAYLOAD_TYPE;

typedef struct payload_t payload_t;

struct payload_t {
  PAYLOAD_TYPE  type;
  void*         body;

  PAYLOAD_TYPE  next_type;
  payload_t*    next;
};

payload_t*  pld_create(PAYLOAD_TYPE type);

#endif //__PAYLOAD_H__

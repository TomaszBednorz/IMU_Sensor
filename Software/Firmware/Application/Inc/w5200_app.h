#ifndef W5200_APP_H_
#define W5200_APP_H_

#include "w5200_config.h"

/*
 *  Functions prototypes
 */
void W5200App_init(void);
void W5200App_reset(void);
int32_t W5200App_loopback(void);
int32_t W5200App_rx(uint8_t* buf);
void W5200App_tx(uint8_t * buf, uint16_t len);

#endif  /* W5200_APP_H_ */


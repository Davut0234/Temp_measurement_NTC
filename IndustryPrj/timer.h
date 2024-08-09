#ifndef TIMER_H_
#define TIMER_H_

void timer_init(void);
uint32_t timer_get_count(void);

#define TIMER_COUNTER						(timer_get_count())

#define TICKS_PER_MS						(1U)
#define TICKS_PER_SEC						(TICKS_PER_MS * 1000U)
#define TICKS_PER_MINUTE					(TICKS_PER_SEC * 60U)

#define MS(x)			(x * TICKS_PER_MS)	
#define SEC(x)			(x * TICKS_PER_SEC)		
#define MIN(x)			(x * TICKS_PER_MINUTE)	

#define TIMER_CHECK_COUNTER(variable, interval)			((TIMER_COUNTER - variable) >= interval)


#endif /* TİMER_H_ */
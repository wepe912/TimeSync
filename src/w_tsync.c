#include "../include/w_tsync.h"


int messure_sys_precision(void){
	ntp_ts	minstep;	/* MINSTEP as ntp_ts */
	ntp_ts	val;		/* current seconds fraction */
	ntp_ts	last;		/* last seconds fraction */
	ntp_ts	ldiff;		/* val - last */
	double	tick;		/* computed tick value */
	double	diff;
	long	repeats;
	long	max_repeats;
	int	changes;
	int	i;		/* log2 precision */

	tick = MAXSTEP;
	max_repeats = 0;
	repeats = 0;
	changes = 0;
	DTONTP_TS(MINSTEP, &minstep);
	printf("sizeof last = %d\n",sizeof(last) );
	//get_systime(&last);
	last = get_systime();
	for (i = 0; i < MAXLOOPS && changes < MINCHANGES; i++) {
		//get_systime(&val);
		val = get_systime();
		//printf("i = %d [last_sec = %d  last_nsec = %d \n",i , last.l_ui,last.l_uf );
		ldiff = val;
		L_SUB(&ldiff, &last);
		last = val;
		if (L_ISGT(&ldiff, &minstep)) {
			max_repeats = max(repeats, max_repeats);
			repeats = 0;
			changes++;
			NTP_STOD(&ldiff, diff);
			tick = min(diff, tick);
		} else {
			repeats++;
		}
	}
	if (changes < MINCHANGES) {
		//msyslog(LOG_ERR, "Fatal error: precision could not be measured (MINSTEP too large?)");
		//exit(1);
		return -1;
	}
	if (tick > 1.) {
		//msyslog(LOG_ERR,"unsupported tick %.3f > 1s ignored", tick);
		return -1;
	}
	/*
	 * Find the nearest power of two.
	 */
	 printf("tick = %0.8f\n",tick);
	for (i = 0; tick <= 1; i--)
		tick *= 2;
	if (tick - 1 > 1 - tick / 2)
		i++;
	sys_precision = (char)i;
}


ntp_ts get_systime(void){
	//if(now = NULL){
	//	return NULL;
	//}
	struct timespec ts;
	ntp_ts result;
	int ret = clock_gettime(CLOCK_REALTIME, &ts);
	if(ret != 0){
		return result;
	}else{
		result.l_i = (int)ts.tv_sec;
		result.l_uf = TVNTOF(ts.tv_nsec);
		//result.l_uf = ts.tv_nsec;
		return result;
		
	}
}
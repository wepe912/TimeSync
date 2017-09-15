#ifndef W_TSYNC_H
#define	W_TSYNC_H
#include "./timeInit.h"
/*
 * deal the first byte in ntp packet
 */

#define VN_MODE(v, m)		((((v) & 7) << 3) | ((m) & 0x7))
#define	PKT_LI_VN_MODE(l, v, m) ((((l) & 3) << 6) | VN_MODE((v), (m)))

#define LOGTOD(a)	ldexp(1., (int)(a)) /* log2 to double */


#define	LEAP_NOWARNING	0x0	/* normal, no leap second warning */
#define	LEAP_ADDSECOND	0x1	/* last minute of day has 61 seconds */
#define	LEAP_DELSECOND	0x2	/* last minute of day has 59 seconds */
#define	LEAP_NOTINSYNC	0x3	/* overload, clock is free running */


#define	MODE_UNSPEC	0	/* unspecified (old version) */
#define	MODE_ACTIVE	1	/* symmetric active mode */
#define	MODE_PASSIVE	2	/* symmetric passive mode */
#define	MODE_CLIENT	3	/* client mode */
#define	MODE_SERVER	4	/* server mode */
#define	MODE_BROADCAST	5	/* broadcast mode */


/*
 * Find the precision of this particular machine
 */
#define MINSTEP		20e-9	/* minimum clock increment (s) */
#define MAXSTEP		1	/* maximum clock increment (s) */
#define MINCHANGES	12	/* minimum number of step samples */
#define MAXLOOPS	((int)(1. / MINSTEP))	/* avoid infinite loop */


 /*
 * Primitive operations on long fixed point values.  If these are
 * reminiscent of assembler op codes it's only because some may
 * be replaced by inline assembler for particular machines someday.
 * These are the (kind of inefficient) run-anywhere versions.
 */
#define FALSE 0
#define FRAC		4294967296.0 		/* 2^32 as a double */
#define u_int32		unsigned int
#define	M_NEG(v_i, v_f)		/* v = -v */ \
	do { \
		(v_f) = ~(v_f) + 1u; \
		(v_i) = ~(v_i) + ((v_f) == 0); \
	} while (FALSE)

#define M_DTONTP_S(d, r_ui, r_uf) 		/* double to ntp_ts */ \
	do { \
		double d_tmp; \
		if ((d_tmp = (d)) < 0) { \
			(r_ui) = (u_int32)(-d_tmp); \
			(r_uf) = (u_int32)(-(d_tmp + (double)(r_ui)) * FRAC); \
			M_NEG((r_ui), (r_uf)); \
		} else { \
			(r_ui) = (u_int32)d_tmp; \
			(r_uf) = (u_int32)((d_tmp - (double)(r_ui)) * FRAC); \
		} \
	} while (FALSE)

#define M_NTP_STOD(r_ui, r_uf, d) 		/* ntp_ts to double */ \
do { \
	u_int32 l_thi, l_tlo; \
	l_thi = (r_ui); l_tlo = (r_uf); \
	if (M_ISNEG(l_thi)) { \
		M_NEG(l_thi, l_tlo); \
		(d) = -((double)l_thi + (double)l_tlo / FRAC); \
	} else { \
		(d) = (double)l_thi + (double)l_tlo / FRAC; \
	} \
} while (FALSE)

#define DTONTP_TS(d, v) M_DTONTP_S((d), (v)->l_ui, (v)->l_uf)
#define NTP_STOD(v, d) 	M_NTP_STOD((v)->l_ui, (v)->l_uf, (d))

#define M_SUB(r_i, r_f, a_i, a_f)	/* r -= a */ \
	do { \
		u_int32 sub_t = (r_f); \
		(r_f) -= (a_f); \
		(r_i) -= (a_i) + ((u_int32)(r_f) > sub_t); \
	} while (FALSE)
#define	L_SUB(r, a)	M_SUB((r)->l_ui, (r)->l_uf, (a)->l_ui, (a)->l_uf)

#define	M_ISGT(a_i, a_f, b_i, b_f)	/* a > b signed */ \
	(((u_int32)((a_i) ^ 0x80000000) > (u_int32)((b_i) ^ 0x80000000)) || \
	  ((a_i) == (b_i) && ((u_int32)(a_f)) > ((u_int32)(b_f))))
#define	L_ISGT(a, b)	M_ISGT((a)->l_i, (a)->l_uf, (b)->l_i, (b)->l_uf)



/*
 * min, min3 and max.  Makes it easier to transliterate the spec without
 * thinking about it.
 */
#define	min(a,b)	(((a) < (b)) ? (a) : (b))
#define	max(a,b)	(((a) > (b)) ? (a) : (b))
#define	min3(a,b,c)	min(min((a),(b)), (c))





//ntp timestamp formate
typedef struct {
	union {
		u_int32 Xl_ui;
		int Xl_i;
	} Ul_i; 			/* sec */
	u_int32 l_uf; 		/* n_sec*/
} ntp_ts;
#define l_ui	Ul_i.Xl_ui	/* unsigned integral part */
#define	l_i	Ul_i.Xl_i		/* signed integral part */


//ntp packet
typedef struct {
	unsigned char	li_vn_mode;	/* peer leap indicator */
	unsigned char	stratum;	/* peer stratum */
	//rfc5905 say poll interval is a signed integer
	char	ppoll;		/* peer poll interval */
	char	precision;	/* peer clock precision */
	unsigned int	rootdelay;	/* roundtrip delay to primary source */
	unsigned int	rootdisp;	/* dispersion to primary source*/
	unsigned int	refid;		/* reference id */
	ntp_ts	reftime;	/* last update time */
	ntp_ts	org;		/* originate time stamp */
	ntp_ts	rec;		/* receive time stamp */
	ntp_ts	xmt;		/* transmit time stamp */
}ntp_pkt;


unsigned char	sys_leap;		/* system leap indicator, use set_sys_leap() to change this */
unsigned char	xmt_leap;		/* leap indicator sent in client requests, set up by set_sys_leap() */
unsigned char	sys_stratum;	/* system stratum */
char	sys_precision;		/* local clock precision (log2 s) */
double	sys_rootdelay;		/* roundtrip delay to primary source */
double	sys_rootdisp;		/* dispersion to primary source */
unsigned int sys_refid;		/* reference id (network byte order) */
ntp_ts	sys_reftime;		/* last update time */

void* tsync_start(void* arg); /* caller create thread to start time server */
//void* tsync(void* arg);
void* agreement_key(void* arg); /* caller create thread to start  agreement_key server */ 

int tsync_client(int tsync_type,const char* server_addr); /* get time from peer or higher stage as a client */

void deal_network_packet(int type,int port); /* deal network packet without thread */ 

void get_systime(ntp_ts* now);/* use clock_gettime get systime and convert into ntp_ts formate */
#endif
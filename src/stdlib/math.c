#include <math.h>

void udiv(u32 n, u32 m, struct qr *qr)
{
	if (n == m) {
		qr->q = 1;
		qr->r = 0;
		return;
	}

	if (n < m) {
		qr->q = 0;
		qr->r = n;
		return;
	}

	for (qr->q = 1;;qr->q++) {
		n -= m;
		if (n < m) {
			qr->r = n;
			break;
		}
	}
}

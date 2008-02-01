//metadoc Collector copyright Steve Dekorte, 2006
//metadoc Collector license BSD revised
//metadoc Collector description A tricolor collector using a Baker treadmill.

#ifndef Collector_DEFINED
#define Collector_DEFINED 1

#include "CollectorMarker.h"

#ifdef __cplusplus
extern "C" {
#endif

#define COLLECTOR_FOREACH(self, v, code) \
	COLLECTMARKER_FOREACH(self->whites,  v, code;); \
	COLLECTMARKER_FOREACH(self->grays,   v, code;); \
	COLLECTMARKER_FOREACH(self->blacks,  v, code;);

typedef enum
{
	COLLECTOR_INITIAL_WHITE,
	COLLECTOR_GRAY,
	COLLECTOR_INITIAL_BLACK,
	COLLECTOR_FREE
} CollectorColor;

typedef int  (CollectorMarkFunc)(void *);
typedef void (CollectorWillFreeFunc)(void *);
typedef void (CollectorFreeFunc)(void *);

typedef struct
{
	List *retainedValues;
	void *markBeforeSweepValue;

	int pauseCount;

	CollectorMarker *blacks;
	CollectorMarker *grays;
	CollectorMarker *whites;
	CollectorMarker *freed;

	float marksPerAlloc;
	float queuedMarks;

	size_t allocated;
	size_t allocatedSweepLevel;
	float allocatedStep;

	CollectorMarkFunc *markFunc;
	CollectorWillFreeFunc *willFreeFunc;
	CollectorFreeFunc *freeFunc;

	long clocksUsed;
	size_t sweepCount;
	int debugOn;
} Collector;

COLLECTOR_API Collector *Collector_new(void);
COLLECTOR_API void Collector_free(Collector *self);

COLLECTOR_API void Collector_check(Collector *self);

COLLECTOR_API void Collector_setMarkBeforeSweepValue_(Collector *self, void *v);

// callbacks

COLLECTOR_API void Collector_setMarkFunc_(Collector *self, CollectorMarkFunc *func);
COLLECTOR_API void Collector_setWillFreeFunc_(Collector *self, CollectorWillFreeFunc *func);
COLLECTOR_API void Collector_setFreeFunc_(Collector *self, CollectorFreeFunc *func);

// marks per alloc

COLLECTOR_API void Collector_setMarksPerAlloc_(Collector *self, float n);
COLLECTOR_API float Collector_marksPerAlloc(Collector *self);

// marks per sweep

COLLECTOR_API void Collector_setAllocatedStep_(Collector *self, float n);
COLLECTOR_API float Collector_allocatedStep(Collector *self);

// debug

COLLECTOR_API void Collector_setDebug_(Collector *self, int b);

// retaining

COLLECTOR_API void *Collector_retain_(Collector *self, void *v);
COLLECTOR_API void Collector_stopRetaining_(Collector *self, void *v);
COLLECTOR_API void Collector_removeAllRetainedValues(Collector *self);

// adding

COLLECTOR_API CollectorMarker *Collector_newMarker(Collector *self);
COLLECTOR_API void Collector_addValue_(Collector *self, void *v);

// collection

COLLECTOR_API void Collector_initPhase(Collector *self);
COLLECTOR_API size_t Collector_sweepPhase(Collector *self);
COLLECTOR_API void Collector_markPhase(Collector *self);

COLLECTOR_API size_t Collector_collect(Collector *self);
COLLECTOR_API size_t Collector_freeAllValues(Collector *self);

// changing colors

#define Collector_shouldMark_(self, v) Collector_makeGrayIfWhite_(self, v)
void Collector_makeGrayIfWhite_(Collector *self, void *v);

void Collector_makeWhite_(Collector *self, CollectorMarker *v);
void Collector_makeGray_(Collector *self, CollectorMarker *v);
void Collector_makeBlack_(Collector *self, CollectorMarker *v);

int Collector_markerIsWhite_(Collector *self, CollectorMarker *m);
int Collector_markerIsGray_(Collector *self, CollectorMarker *m);
int Collector_markerIsBlack_(Collector *self, CollectorMarker *m);

COLLECTOR_API char *Collector_colorNameFor_(Collector *self, void *v);

void *Collector_value_addingRefTo_(Collector *self, void *v, void *ref);

// pause/resume stack

COLLECTOR_API void Collector_pushPause(Collector *self);
COLLECTOR_API void Collector_popPause(Collector *self);
COLLECTOR_API int Collector_isPaused(Collector *self);
COLLECTOR_API double Collector_timeUsed(Collector *self);

#include "Collector_inline.h"

#ifdef __cplusplus
}
#endif
#endif

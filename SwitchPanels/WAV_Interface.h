/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQ_PANEL                       1       /* callback function: OnFreq */
#define  FREQ_PANEL_GRAPH_SELECT          2       /* control type: binary, callback function: (none) */
#define  FREQ_PANEL_FILTER_SELECT         3       /* control type: binary, callback function: (none) */
#define  FREQ_PANEL_SWITCHPANELS          4       /* control type: binary, callback function: OnPanels */
#define  FREQ_PANEL_GRAPH_FILT_FREQ       5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_FILT            6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_GRAPH_RAW_FREQ        7       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_FREQ_PEAK             8       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_POW_PEAK              9       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_GRAPH_RAW             10      /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_NR_POINTS             11      /* control type: ring, callback function: OnNrPointsUpdate */
#define  FREQ_PANEL_STOP                  12      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_NEXT                  13      /* control type: command, callback function: OnFreqNext */
#define  FREQ_PANEL_START                 14      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_APPLY                 15      /* control type: command, callback function: OnFreqApply */
#define  FREQ_PANEL_PREV                  16      /* control type: command, callback function: OnFreqPrev */

#define  WAVE_PANEL                       2       /* callback function: OnWave */
#define  WAVE_PANEL_LOADBUTTON            2       /* control type: command, callback function: OnLoad */
#define  WAVE_PANEL_SWITCHPANELS          3       /* control type: binary, callback function: OnPanels */
#define  WAVE_PANEL_GRAPH_2               4       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_GRAPH                 5       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_GRAPH_HISTOGRAM       6       /* control type: graph, callback function: (none) */
#define  WAVE_PANEL_KURTOSIS              7       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_SKEWNESS              8       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_ZERO                  9       /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MEDIAN                10      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MEAN                  11      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_DISPERSION            12      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MIN_INDEX             13      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MAX_INDEX             14      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MINIM                 15      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_STOP                  16      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_START                 17      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MAXIM                 18      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_NEXT                  19      /* control type: command, callback function: OnNext */
#define  WAVE_PANEL_APPLY                 20      /* control type: command, callback function: OnApply */
#define  WAVE_PANEL_GEN_ENVELOPE          21      /* control type: command, callback function: OnEnvelope */
#define  WAVE_PANEL_GEN_DERIVATIVE        22      /* control type: command, callback function: OnDev */
#define  WAVE_PANEL_PREV                  23      /* control type: command, callback function: OnPrev */
#define  WAVE_PANEL_ALPHA                 24      /* control type: numeric, callback function: (none) */
#define  WAVE_PANEL_MEDIAN_TYPE           25      /* control type: ring, callback function: (none) */
#define  WAVE_PANEL_SELECTION             26      /* control type: ring, callback function: GraphSelection */
#define  WAVE_PANEL_FILTERS               27      /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK GraphSelection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApply(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnEnvelope(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreq(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqApply(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoad(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNrPointsUpdate(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanels(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnWave(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "WAV_Interface.h"
#include "toolbox.h"
#include <stdio.h>
#include <math.h>

// Constants
#define SAMPLE_RATE		0
#define NPOINTS			1

// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements
int npoints = 0;
int MedVal = 0;
double alpha = 0.01;
double sampleRate = 0.0;
static double *waveData = 0;
static double *rawIn = 0;
static double *anvelopa = 0;
static double *signalSeconds = 0;
static double *filters = 0;
static double *filterFreq = 0;
static double *filterRes = 0;
static double *dev = 0;
static double *anvRes = 0;
static double maxVal = 0.0;
static double minVal = 0.0;
static int maxIndex = 0;
static int minIndex = 0;
static double dispersion = 0.0;
static double mean = 0.0;
static double median = 0.0;
static int zeros = 0;
static int histogram[100];
static double axis[100];
static double skewness = 0.0;
static double kurtosis = 0.0;
static int n = 256;
static double mediaSK = 0.0;
// Panels
static int wavePanel = 0;
static int frequencyPanel = 0;

//Variables for functions
static int loading = 0;
static int Start = 0;
static int Stop = 1;
static int selection = 0;
static int nElem = 0;
//static int BothGraphics = 0;
static int filterOn = 0;
static int firstTime = 1;
static int med = 0, order = 0;

// Variables for Frequency
WindowConst winConst;
int win;
int graph;
double winParam = 0;
double df;
static double* Auto_Spec; 
static double* spectrumFilter;
static double freqPeak;
static double powerPeak;


// date&time variables
int year = 0;
int month = 0;
int day = 0;
int hour = 0;
int minute = 0;
int second = 0;
int imgHandle;  
char fileName[256] = {0};
BOOL ** weSavedGraph;

int main (int argc, char *argv[]){
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (wavePanel = LoadPanel (0, "WAV_Interface.uir", WAVE_PANEL));
	errChk (frequencyPanel = LoadPanel (0, "WAV_Interface.uir", FREQ_PANEL));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (wavePanel));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (wavePanel > 0)
		DiscardPanel (wavePanel);
	return 0;
}

int CVICALLBACK OnWave (int panel, int event, void *callbackData,
						 int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			// activarea butonului pentru inchiderea ferestrei
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK OnFreq (int panel, int event, void *callbackData,
						int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			// activarea butonului pentru inchiderea ferestrei
			QuitUserInterface(0);
			break;
	}
	return 0;
}


int CVICALLBACK OnLoad (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			if(loading == 0)
			{
				loading = 1;
				SetCtrlAttribute (wavePanel, WAVE_PANEL_LOADBUTTON, ATTR_LABEL_TEXT, "__Unload");
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GRAPH, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GRAPH_HISTOGRAM, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MAXIM, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MINIM, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MEDIAN, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_ZERO, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_DISPERSION, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MEAN, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MAX_INDEX, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MIN_INDEX, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GRAPH_2, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_PREV, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_NEXT, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_START, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_STOP, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel,	WAVE_PANEL_SWITCHPANELS, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GEN_DERIVATIVE, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GEN_ENVELOPE, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_SELECTION, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_FILTERS, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_ALPHA, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MEDIAN_TYPE, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_APPLY, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_SELECTION, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_SKEWNESS, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_KURTOSIS, ATTR_DIMMED, !loading);
				//incarc informatiile privind rata de esantionare si numarul de valori
				FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				sampleRate = waveInfo[SAMPLE_RATE];
				npoints = waveInfo[NPOINTS];
				
				//alocare memorie pentru numarul de puncte
				waveData = (double*)calloc(sampleRate*6, sizeof(double));
				rawIn = (double*)calloc(sampleRate*6, sizeof(double));
				Auto_Spec = (double*)calloc(npoints/2, sizeof(double));
				//incarcare din fisierul .txt in memorie (vector)		
				if (6*sampleRate > npoints){
				    for (int i = 0; i < sampleRate * 6; i++) {
				        waveData[i] = 0.0;
				    }
				}
				// weSavedGraph[0] -> RawData, 1024 elem -> 64 (128 for Welch + FlatTop)
				// weSavedGraph[1] -> RawData, 2048 elem -> 32 (64 for Welch + FlatTop)
				// weSavedGraph[2] -> RawData, 4096 elem -> 16 (32 for Welch + FlatTop)
				// weSavedGraph[3] -> RawData, 8192 elem -> 8 (16 for Welch + FlatTop)
				// weSavedGraph[4] -> RawData, 16384 elem -> 4 (8 for Welch + FlatTop)
				// weSavedGraph[5] -> FilterData, 1024 elem -> 64 (128 for Welch + FlatTop)
				// weSavedGraph[6] -> FilterData, 2048 elem -> 32 (64 for Welch + FlatTop)
				// weSavedGraph[7] -> FilterData, 4096 elem -> 16 (32 for Welch + FlatTop)
				// weSavedGraph[8] -> FilterData, 8192 elem -> 8 (16 for Welch + FlatTop)
				// weSavedGraph[9] -> FilterData, 16384 elem -> 4 (8 for Welch + FlatTop)
				// weSavedGraph[10] -> RawDataEntire + Partitioned;
				// weSavedGraph[11] -> FilteredEntire + Partitioned;
				
				weSavedGraph = (BOOL **) calloc(12, sizeof(BOOL*));
				weSavedGraph[0] = calloc(128, sizeof(BOOL));
				weSavedGraph[1] = calloc(64, sizeof(BOOL));
				weSavedGraph[2] = calloc(32, sizeof(BOOL));
				weSavedGraph[3] = calloc(16, sizeof(BOOL));
				weSavedGraph[4] = calloc(8, sizeof(BOOL));
				weSavedGraph[5] = calloc(128, sizeof(BOOL));
				weSavedGraph[6] = calloc(64, sizeof(BOOL));
				weSavedGraph[7] = calloc(32, sizeof(BOOL));
				weSavedGraph[8] = calloc(16, sizeof(BOOL));
				weSavedGraph[9] = calloc(8, sizeof(BOOL));
				weSavedGraph[10] = calloc(7, sizeof(BOOL));
				weSavedGraph[11] = calloc(7, sizeof(BOOL));
				
				FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);	
				Copy1D(waveData, 6*sampleRate , rawIn);
				//anvelopa
				//alocare memorie pentru numarul de puncte
				anvelopa = (double *) calloc(sampleRate * 6, sizeof(double));  
				//incarcare din fisierul .txt in memorie (vector)
				FileToArray("anvelopa.txt", anvelopa, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
				
				DeleteGraphPlot(panel, WAVE_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
				PlotY(panel, WAVE_PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				
				//calculare valoare minima si valoare maxima
				MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
				//calculare valoare medie
				Mean(waveData,npoints,&mean);
				//calculare dispersie
				StdDev(waveData, npoints, &mean, &dispersion);
				//calculare mediana
				Median(waveData, npoints, &median);
				//calculare zerouri
				zeros = 0;
				for (int i=0; i < npoints-1; i++)
					if ( (waveData[i] <= 0 && waveData[i+1] >= 0) || (waveData[i] >= 0 && waveData[i+1] <= 0))
						zeros++;
				
				//calculare skewness
				skewness = 0.0;
				double skTop = 0.0;
				double skDown = 0.0;
				
				Mean(waveData,n,&mediaSK);
				
				for(int i = 0; i < n; i++){
					skTop += pow((waveData[i] - mediaSK), 3);
					skDown += pow((waveData[i] 	- mediaSK), 2);
				}

				skewness = ((1.0/n)*skTop) / pow(1.0/n * skDown, 3/2);
				
				//calculare kurtosis
				kurtosis = 0.0;
				double ktTop = 0.0;
			    for (int i = 0; i < n; ++i) {
			        ktTop += pow((waveData[i] - mediaSK), 4);
				}
			    // am folosit acelasi 1.0/n*skdown pentru ca se calculeaza dupa aceeasi formula 
				// (are aceeasi valoare) cu ce trebuia de calculat :)
				kurtosis = ((1.0/n)*ktTop/pow(1.0/n*skDown,2));

				//setare valori
				SetCtrlVal(panel, WAVE_PANEL_MINIM, minVal);
				SetCtrlVal(panel, WAVE_PANEL_MAXIM, maxVal);
				SetCtrlVal(panel, WAVE_PANEL_MIN_INDEX, minIndex);
				SetCtrlVal(panel, WAVE_PANEL_MAX_INDEX, maxIndex);
				SetCtrlVal(panel, WAVE_PANEL_DISPERSION, dispersion);
				SetCtrlVal(panel, WAVE_PANEL_MEAN, mean);
				SetCtrlVal(panel, WAVE_PANEL_MEDIAN, median);
				SetCtrlVal(panel, WAVE_PANEL_ZERO, zeros);
				SetCtrlVal(panel, WAVE_PANEL_SKEWNESS, skewness);
				SetCtrlVal(panel, WAVE_PANEL_KURTOSIS, kurtosis);
				
				//afisare histograma
				DeleteGraphPlot (panel, WAVE_PANEL_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
				Histogram(waveData, npoints, minVal, maxVal, histogram, axis, 10);
				PlotXY (panel, WAVE_PANEL_GRAPH_HISTOGRAM, axis,  histogram, 10, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GEN_ENVELOPE, ATTR_DIMMED, !loading);
			}
			else{
				loading = 0;
				SetCtrlAttribute (wavePanel, WAVE_PANEL_LOADBUTTON, ATTR_LABEL_TEXT, "__Load");
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GRAPH, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GRAPH_HISTOGRAM, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MAXIM, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MINIM, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MEDIAN, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_ZERO, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_DISPERSION, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MEAN, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MAX_INDEX, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MIN_INDEX, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GRAPH_2, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_PREV, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_NEXT, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_START, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_STOP, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel,	WAVE_PANEL_SWITCHPANELS, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GEN_DERIVATIVE, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_GEN_ENVELOPE, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_SELECTION, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_FILTERS, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_ALPHA, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_MEDIAN_TYPE, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_APPLY, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_SELECTION, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_SKEWNESS, ATTR_DIMMED, !loading);
				SetCtrlAttribute(wavePanel, WAVE_PANEL_KURTOSIS, ATTR_DIMMED, !loading);
				DeleteGraphPlot(wavePanel, WAVE_PANEL_GRAPH,-1,VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot(wavePanel, WAVE_PANEL_GRAPH_2,-1,VAL_IMMEDIATE_DRAW);
				DeleteGraphPlot(wavePanel, WAVE_PANEL_GRAPH_HISTOGRAM,-1,VAL_IMMEDIATE_DRAW);
				for( int i = 0; i < 12; i++){
					 free(weSavedGraph[i]);
				}
				free(weSavedGraph);
			}
	}
	return 0;
}

// pentru a face trecerea dintre "paginile" semnalului, am considerat 2 variabile globale, "Start" si "Stop"
// ele modificandu-se atat cand parcurgem semnalul de la stanga la dreapta, dar si in sens invers

int CVICALLBACK OnPrev (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, WAVE_PANEL_STOP, &Stop);
			GetCtrlVal(panel, WAVE_PANEL_START, &Start);
			
			if(Stop > 1 && Start <= 6)
			{
				Start = Start - 1;
				Stop = Stop - 1;
				GraphSelection(panel, WAVE_PANEL_PREV,EVENT_COMMIT,0,0,0);
				SetCtrlVal(panel, WAVE_PANEL_START, Start);
				SetCtrlVal(panel, WAVE_PANEL_STOP, Stop);
			}
			break;
}
	return 0;
}

int CVICALLBACK OnNext (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:		
			GetCtrlVal(panel, WAVE_PANEL_STOP, &Stop);
			GetCtrlVal(panel, WAVE_PANEL_START, &Start);
			
				
			if(Stop >= 0 && Start < 5)
			{
				Start = Start + 1;
				Stop = Stop + 1;
				GraphSelection(panel, WAVE_PANEL_PREV,EVENT_COMMIT,0,0,0);
				SetCtrlVal(panel, WAVE_PANEL_START, Start);
				SetCtrlVal(panel, WAVE_PANEL_STOP, Stop);
			}
			break;
	}
	return 0;
}

int CVICALLBACK GraphSelection (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2){

	
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(wavePanel, WAVE_PANEL_SELECTION, &selection);
			switch (selection){
				// calculez si afisez valorile pe primul grafic al intregului semnal incarcat
				case 0:
					DeleteGraphPlot(panel, WAVE_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
					DeleteGraphPlot(panel,WAVE_PANEL_GRAPH_2, -1, VAL_IMMEDIATE_DRAW);
					PlotY(panel, WAVE_PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
					//calculare valoare minima si valoare maxima
					MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
					//calculare valoare medie
					Mean(waveData,npoints,&mean);
					//calculare dispersie
					StdDev(waveData, npoints, &mean, &dispersion);
					//calculare mediana
					Median(waveData, npoints, &median);
					//calculare zerouri
					zeros = 0;
					for (int i=0; i < npoints-1; i++)
						if ( (waveData[i] < 0 && waveData[i+1] > 0) || (waveData[i] > 0 && waveData[i+1] < 0))
							zeros++;
					
					//setare valori
					SetCtrlVal(panel, WAVE_PANEL_MINIM, minVal);
					SetCtrlVal(panel, WAVE_PANEL_MAXIM, maxVal);
					SetCtrlVal(panel, WAVE_PANEL_MIN_INDEX, minIndex);
					SetCtrlVal(panel, WAVE_PANEL_MAX_INDEX, maxIndex);
					SetCtrlVal(panel, WAVE_PANEL_DISPERSION, dispersion);
					SetCtrlVal(panel, WAVE_PANEL_MEAN, mean);
					SetCtrlVal(panel, WAVE_PANEL_MEDIAN, median);
					SetCtrlVal(panel, WAVE_PANEL_ZERO, zeros);
					
					//afisare histograma
					DeleteGraphPlot (panel, WAVE_PANEL_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
					Histogram(waveData, npoints, minVal, maxVal, histogram, axis, 10);
					PlotXY (panel, WAVE_PANEL_GRAPH_HISTOGRAM, axis,  histogram, 10, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
					
					if(!weSavedGraph[10][0]){
						weSavedGraph[10][0] = TRUE;
						//luam data si ora
						GetSystemDate(&month, &day, &year);
						GetSystemTime(&hour, &minute, &second);
						
						//setam numele cu care salvam imaginea
						sprintf(fileName, "RAW_DATA_ENTIRE_%4d.%2d.%2d_%2d-%2d-%2d.jpg", year, month, day, hour, minute, second);
						//salvam imaginea
						GetCtrlDisplayBitmap(panel,WAVE_PANEL_GRAPH,1,&imgHandle);
						SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
						DiscardBitmap(imgHandle);
					}
					
					break;
					
				// calculez si afisez valorile pe primul grafic al semnalului incarcat, insa doar pe bucati / "pagini"
				// trecerea intre acestea facandu-se cu ajutorul butoanelor de "Next" si "Prev"
				case 1:
					signalSeconds = waveData + (int)(Start * sampleRate);
					DeleteGraphPlot(wavePanel, WAVE_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
					DeleteGraphPlot (panel,WAVE_PANEL_GRAPH_2, -1, VAL_IMMEDIATE_DRAW);
					PlotY(panel, WAVE_PANEL_GRAPH, signalSeconds, sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
					//calculare valoare minima si valoare maxima
					MaxMin1D(signalSeconds,sampleRate,&maxVal,&maxIndex,&minVal,&minIndex);
					//calculare valoare medie
					Mean(signalSeconds,sampleRate,&mean);
					//calculare dispersie
					StdDev(signalSeconds, sampleRate, &mean, &dispersion);
					//calculare mediana
					Median(signalSeconds, sampleRate, &median);
					//calculare zerouri
					zeros = 0;
					for (int i=0; i < sampleRate-1; i++)
						if ( (signalSeconds[i] < 0 && signalSeconds[i+1] > 0) || (signalSeconds[i] > 0 &&signalSeconds[i+1] < 0))
							zeros++;
					
					//setare valori
					SetCtrlVal(panel, WAVE_PANEL_MINIM, minVal);
					SetCtrlVal(panel, WAVE_PANEL_MAXIM, maxVal);
					SetCtrlVal(panel, WAVE_PANEL_MIN_INDEX, minIndex);
					SetCtrlVal(panel, WAVE_PANEL_MAX_INDEX, maxIndex);
					SetCtrlVal(panel, WAVE_PANEL_DISPERSION, dispersion);
					SetCtrlVal(panel, WAVE_PANEL_MEAN, mean);
					SetCtrlVal(panel, WAVE_PANEL_MEDIAN, median);
					SetCtrlVal(panel, WAVE_PANEL_ZERO, zeros);
					
					//afisare histograma
					DeleteGraphPlot (panel, WAVE_PANEL_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
					Histogram(signalSeconds, sampleRate, minVal, maxVal, histogram, axis, 10);
					PlotXY (panel, WAVE_PANEL_GRAPH_HISTOGRAM, axis,  histogram, 10, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
					
					// SAVED &Start , TO DO FOR FILTERED ASWELL!@!!!!!!!
					
					if(!weSavedGraph[10][Stop]){
						weSavedGraph[10][Stop] = TRUE;
						//luam data si ora
						GetSystemDate(&month, &day, &year);
						GetSystemTime(&hour, &minute, &second);
						
						//setam numele cu care salvam imaginea
						sprintf(fileName, "RAW_DATA_PARTITIONED_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg", Start, Stop, year, month, day, hour, minute, second);
						//salvam imaginea
						GetCtrlDisplayBitmap(panel,WAVE_PANEL_GRAPH,1,&imgHandle);
						SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
						DiscardBitmap(imgHandle);
					}
					break;
					
				//calculez si afisez valorile pe al doilea grafic al semnalului filtrat + afisez primul grafic cu semnalul initial.
				case 2:
					GetCtrlVal(wavePanel, WAVE_PANEL_FILTERS, &filterOn);	
					GetCtrlVal(panel,WAVE_PANEL_MEDIAN_TYPE, &MedVal);
					GetCtrlVal(panel,WAVE_PANEL_ALPHA, &alpha);
					if(filterOn == 1){ // filtru mediere
						double sum = 0;
						if(med == 0){
							filters = (double *) calloc(sampleRate * 6, sizeof(double));
							for (int i = 0; i < sampleRate * 6; i++){
								for (int k = 0; k < MedVal; k++){
									if ((i - k) >= 0){
										sum += waveData[i-k];	
									}
								}
								filters[i] = sum/MedVal;
								sum = 0;	
							}
							med = 1;
							order = 0;
						}
					}
					else if(filterOn == 2){ // filtru element de ordin 1	
						if(order == 0){
							filters = (double *) calloc(sampleRate * 6, sizeof(double));
							filters[0] = waveData[0];
							for(int i = 1; i < npoints; i++){
								filters[i] = (1-alpha)*filters[i-1] + alpha*waveData[i];
							}
							med = 0;
							order = 1;
						}
					}
					
					//calculare valoare minima si valoare maxima
					MaxMin1D(filters,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
					//calculare valoare medie
					Mean(filters,npoints,&mean);
					//calculare dispersie
					StdDev(filters, npoints, &mean, &dispersion);
					//calculare mediana
					Median(filters, npoints, &median);
					//calculare zerouri
					zeros = 0;
					for (int i=0; i < npoints-1; i++)
						if ( (filters[i] < 0 && filters[i+1] > 0) || (filters[i] > 0 && filters[i+1] < 0))
							zeros++;
					
					//setare valori
					SetCtrlVal(panel, WAVE_PANEL_MINIM, minVal);
					SetCtrlVal(panel, WAVE_PANEL_MAXIM, maxVal);
					SetCtrlVal(panel, WAVE_PANEL_MIN_INDEX, minIndex);
					SetCtrlVal(panel, WAVE_PANEL_MAX_INDEX, maxIndex);
					SetCtrlVal(panel, WAVE_PANEL_DISPERSION, dispersion);
					SetCtrlVal(panel, WAVE_PANEL_MEAN, mean);
					SetCtrlVal(panel, WAVE_PANEL_MEDIAN, median);
					SetCtrlVal(panel, WAVE_PANEL_ZERO, zeros);
					
					//afisare histograma
					DeleteGraphPlot (panel, WAVE_PANEL_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
					Histogram(filters, npoints, minVal, maxVal, histogram, axis, 10);
					PlotXY (panel, WAVE_PANEL_GRAPH_HISTOGRAM, axis,  histogram, 10, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);				
					
					// Afisare Semnal Filtrat + Semnal Intreg
					DeleteGraphPlot(panel, WAVE_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
					DeleteGraphPlot (panel,WAVE_PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
					PlotY(panel, WAVE_PANEL_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					PlotY(panel, WAVE_PANEL_GRAPH_2, filters ,npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					
					if(!weSavedGraph[11][0]){
						weSavedGraph[11][0] = TRUE;
						//luam data si ora
						GetSystemDate(&month, &day, &year);
						GetSystemTime(&hour, &minute, &second);
						
						//setam numele cu care salvam imaginea
						sprintf(fileName, "FILTERED_DATA_ENTIRE_%4d.%2d.%2d_%2d-%2d-%2d.jpg",year, month, day, hour, minute, second);
						//salvam imaginea
						GetCtrlDisplayBitmap(panel,WAVE_PANEL_GRAPH_2,1,&imgHandle);
						SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
						DiscardBitmap(imgHandle);
					}
					
					break;	
				
				//calculez si afisez valorile pe al doilea grafic al semnalului filtrat, insa doar pe bucati / "pagini" + semnalul initial pe bucati.
				case 3:
					GetCtrlVal(wavePanel, WAVE_PANEL_FILTERS, &filterOn);	
					GetCtrlVal(panel,WAVE_PANEL_MEDIAN_TYPE, &MedVal);
					GetCtrlVal(panel,WAVE_PANEL_ALPHA, &alpha);
					if(filterOn == 1){ // filtru mediere
						double sum = 0;
						if(med == 0){
							filters = (double *) calloc(sampleRate * 6, sizeof(double));
							for (int i = 0; i < sampleRate * 6; i++){
								for (int k = 0; k < MedVal; k++){
									if ((i - k) >= 0){
										sum += waveData[i-k];	
									}
								}
								filters[i] = sum/MedVal;
								sum = 0;	
							}
							med = 1;
							order = 0;
						}
					}
					else if(filterOn == 2){ // filtru element de ordin 1	
						if(order == 0){
							filters = (double *) calloc(sampleRate * 6, sizeof(double));
							filters[0] = waveData[0];
							for(int i = 1; i < npoints; i++){
								filters[i] = (1-alpha)*filters[i-1] + alpha*waveData[i];
							}
							med = 0;
							order = 1;
						}
					}		
					
					filterRes = filters + (int)(Start * sampleRate);
					signalSeconds = waveData + (int)(Start * sampleRate);	
					
					//calculare valoare minima si valoare maxima
					MaxMin1D(filterRes,sampleRate,&maxVal,&maxIndex,&minVal,&minIndex);
					//calculare valoare medie
					Mean(filterRes,sampleRate,&mean);
					//calculare dispersie
					StdDev(filterRes, sampleRate, &mean, &dispersion);
					//calculare mediana
					Median(filterRes, sampleRate, &median);
					//calculare zerouri
					zeros = 0;
					for (int i=0; i < sampleRate-1; i++)
						if ( (filterRes[i] < 0 && filterRes[i+1] > 0) || (filterRes[i] > 0 && filterRes[i+1] < 0))
							zeros++;
					
					//setare valori
					SetCtrlVal(panel, WAVE_PANEL_MINIM, minVal);
					SetCtrlVal(panel, WAVE_PANEL_MAXIM, maxVal);
					SetCtrlVal(panel, WAVE_PANEL_MIN_INDEX, minIndex);
					SetCtrlVal(panel, WAVE_PANEL_MAX_INDEX, maxIndex);
					SetCtrlVal(panel, WAVE_PANEL_DISPERSION, dispersion);
					SetCtrlVal(panel, WAVE_PANEL_MEAN, mean);
					SetCtrlVal(panel, WAVE_PANEL_MEDIAN, median);
					SetCtrlVal(panel, WAVE_PANEL_ZERO, zeros);
					
					//afisare histograma
					DeleteGraphPlot (panel, WAVE_PANEL_GRAPH_HISTOGRAM, -1, VAL_DELAYED_DRAW);
					Histogram(filterRes, sampleRate, minVal, maxVal, histogram, axis, 10);
					PlotXY (panel, WAVE_PANEL_GRAPH_HISTOGRAM, axis,  histogram, 10, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);				
					
					
					// afisare semnal
					DeleteGraphPlot(panel, WAVE_PANEL_GRAPH, -1, VAL_IMMEDIATE_DRAW);
					DeleteGraphPlot (panel,WAVE_PANEL_GRAPH_2, -1, VAL_DELAYED_DRAW);
					PlotY(panel, WAVE_PANEL_GRAPH, signalSeconds, sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					PlotY(panel, WAVE_PANEL_GRAPH_2, filterRes ,sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
					SetCtrlAttribute(wavePanel,WAVE_PANEL_GRAPH_2, ATTR_XAXIS_OFFSET, Start * sampleRate);
					SetCtrlAttribute(wavePanel,WAVE_PANEL_GRAPH, ATTR_XAXIS_OFFSET, Start * sampleRate);
					
					if(!weSavedGraph[11][Stop]){
						weSavedGraph[11][Stop] = TRUE;
						//luam data si ora
						GetSystemDate(&month, &day, &year);
						GetSystemTime(&hour, &minute, &second);
						
						//setam numele cu care salvam imaginea
						sprintf(fileName, "FILTERED_DATA_PARTITIONED_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg", Start, Stop, year, month, day, hour, minute, second);
						//salvam imaginea
						GetCtrlDisplayBitmap(panel,WAVE_PANEL_GRAPH_2,1,&imgHandle);
						SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
						DiscardBitmap(imgHandle);
					}
					
					break;
			}
	}
	return 0;
}


int CVICALLBACK FreqGraphSelection (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, FREQ_PANEL_FILTER_SELECT, &win);
			GetCtrlVal(panel, FREQ_PANEL_GRAPH_SELECT, &graph);	
			GetCtrlVal(panel, FREQ_PANEL_NR_POINTS, &nElem);
			if(firstTime){			
				double sum = 0.0;
				// aplicare filtru mediere pe 32 elemente
				// salvare in variabila filterFreq
				filterFreq = (double *) calloc(sampleRate*6, sizeof(double));
				spectrumFilter = (double *) calloc(sampleRate*6, sizeof(double));
				for( int i = 0; i < 16384; i++){
					filterFreq[i] = 0.0;
				}
				Copy1D(rawIn, sampleRate*6, spectrumFilter);
				for (int i = 0; i < sampleRate * 6; i++){
					for (int k = 0; k < 32; k++){
						if ((i - k) >= 0){
							sum += spectrumFilter[i-k];	
						}
					}
					filterFreq[i] = sum/32;
					sum = 0.0;						
				}
				firstTime = 0;
				InvCh_HPF (filterFreq, sampleRate * 6, sampleRate, 750, 80.0, 17, filterFreq);			
			}
			filterRes = filterFreq + (int)(Start * nElem);
			signalSeconds = spectrumFilter + (int)(Start * nElem);
			if(graph == 0){ // RAW
				if(win == 1){ // FLAT-TOP 
					ScaledWindowEx (signalSeconds, nElem, FLATTOP, winParam, &winConst);
				}else{ // WELCH 
					ScaledWindowEx (signalSeconds, nElem, WELCH, winParam, &winConst);
				}
				AutoPowerSpectrum (signalSeconds, nElem, 1./sampleRate, Auto_Spec, &df);
			}else{ // FILTERED
				if(win == 1){ // FLAT-TOP 
					ScaledWindowEx (filterRes, nElem, FLATTOP, winParam, &winConst);
				}else{ // WELCH 
					ScaledWindowEx (filterRes, nElem, WELCH, winParam, &winConst);
				}
				AutoPowerSpectrum (filterRes, nElem, 1./sampleRate, Auto_Spec, &df);
			}
		
			DeleteGraphPlot (panel, FREQ_PANEL_GRAPH_RAW, -1, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (panel, FREQ_PANEL_GRAPH_RAW_FREQ, -1, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (panel, FREQ_PANEL_GRAPH_FILT, -1, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (panel, FREQ_PANEL_GRAPH_FILT_FREQ, -1, VAL_IMMEDIATE_DRAW);
			
			PowerFrequencyEstimate (Auto_Spec, nElem/2, -1.0, winConst, df, 7, &freqPeak, &powerPeak);

			SetCtrlVal(panel, FREQ_PANEL_FREQ_PEAK, freqPeak);
			SetCtrlVal(panel, FREQ_PANEL_POW_PEAK, powerPeak);
			
			int idx = (int)(log(nElem/1024) / log(2));
			
			// TO FIX FLAT TOP , WELCH AND PROJECT DONE!!!!!!!!
			
			if(graph == 0 ) { // RAW
				PlotY(panel, FREQ_PANEL_GRAPH_RAW, signalSeconds, nElem, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				PlotWaveform (panel, FREQ_PANEL_GRAPH_RAW_FREQ, Auto_Spec, nElem/2, VAL_DOUBLE, 1.0, 0.0, 0.0, df, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_DK_GREEN);
				if(!weSavedGraph[idx][2 * Start + win]){
					weSavedGraph[idx][2 * Start + win] = TRUE;
					//luam data si ora
					GetSystemDate(&month, &day, &year);
					GetSystemTime(&hour, &minute, &second);
					
					//setam numele cu care salvam imaginea
					if(win == 1){
						sprintf(fileName, "FLAT_TOP_DATA_SPECTRUM_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}else{
						sprintf(fileName, "WELCH_DATA_SPECTRUM_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}
					//salvam imaginea
					GetCtrlDisplayBitmap(panel,FREQ_PANEL_GRAPH_RAW_FREQ,1,&imgHandle);
					SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
					DiscardBitmap(imgHandle);
					
					//setam numele cu care salvam imaginea
					if(win == 1){
						sprintf(fileName, "FLAT_TOP_DATA_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}else{
						sprintf(fileName, "WELCH_DATA_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}
					//salvam imaginea
					GetCtrlDisplayBitmap(panel,FREQ_PANEL_GRAPH_RAW,1,&imgHandle);
					SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
					DiscardBitmap(imgHandle);							
				}
			}else{ // FILTERED
				PlotY(panel, FREQ_PANEL_GRAPH_FILT, filterRes, nElem, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
				PlotWaveform (panel, FREQ_PANEL_GRAPH_FILT_FREQ, Auto_Spec, nElem/2, VAL_DOUBLE, 1.0, 0.0, 0.0, df, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_DK_GREEN);
				if(!weSavedGraph[idx+5][2 * Start + win]){
					weSavedGraph[idx+5][2 * Start + win] = TRUE;
					//luam data si ora
					GetSystemDate(&month, &day, &year);
					GetSystemTime(&hour, &minute, &second);
					
					if(win == 1){
						sprintf(fileName, "FILTERED_FLAT_TOP_DATA_SPECTRUM_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}else{
						sprintf(fileName, "FILTERED_WELCH_DATA_SPECTRUM_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}
					//salvam imaginea
					GetCtrlDisplayBitmap(panel,FREQ_PANEL_GRAPH_FILT_FREQ,1,&imgHandle);
					SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
					DiscardBitmap(imgHandle);
					
					if(win == 1){
						sprintf(fileName, "FILTERED_FLAT_TOP_DATA_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}else{
						sprintf(fileName, "FILTERED_WELCH_DATA_FOR_%d_ELEMENTS_INTERVAL_%d-%d_%4d.%2d.%2d_%2d-%2d-%2d.jpg",nElem, Start, Stop, year, month, day, hour, minute, second);
					}//salvam imaginea
					GetCtrlDisplayBitmap(panel,FREQ_PANEL_GRAPH_FILT,1,&imgHandle);
					SaveBitmapToJPEGFile(imgHandle,fileName,JPEG_PROGRESSIVE,100);
					DiscardBitmap(imgHandle);
				}
			}

			break;
	}
	return 0;
}


int CVICALLBACK OnFreqPrev (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, FREQ_PANEL_STOP, &Stop);
			GetCtrlVal(panel, FREQ_PANEL_START, &Start);
			
			if(Stop > 1)
			{
				Start = Start - 1;
				Stop = Stop - 1;
				FreqGraphSelection(panel, FREQ_PANEL_PREV,EVENT_COMMIT,0,0,0);
				SetCtrlVal(panel, FREQ_PANEL_START, Start);
				SetCtrlVal(panel, FREQ_PANEL_STOP, Stop);
			}
			break;
}
	return 0;
}

int CVICALLBACK OnFreqNext (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:		
			GetCtrlVal(panel, FREQ_PANEL_STOP, &Stop);
			GetCtrlVal(panel, FREQ_PANEL_START, &Start);
			GetCtrlVal(panel, FREQ_PANEL_NR_POINTS, &nElem);
				
			if( ((Stop + 1) * nElem) < (6 * sampleRate) )
			{
				Start = Start + 1;
				Stop = Stop + 1;
				FreqGraphSelection(panel, FREQ_PANEL_PREV,EVENT_COMMIT,0,0,0);
				SetCtrlVal(panel, FREQ_PANEL_START, Start);
				SetCtrlVal(panel, FREQ_PANEL_STOP, Stop);
			}
			break;
	}
	return 0;
}


int CVICALLBACK OnFreqApply (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:		
			FreqGraphSelection(panel, FREQ_PANEL_PREV,EVENT_COMMIT,0,0,0);
			break;
	}
	return 0;
}


int CVICALLBACK OnNrPointsUpdate (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			Start = 0;
			Stop = 1;
			SetCtrlVal(panel, FREQ_PANEL_START, Start);
			SetCtrlVal(panel, FREQ_PANEL_STOP, Stop);
			FreqGraphSelection(panel, FREQ_PANEL_PREV,EVENT_COMMIT,0,0,0);
	}
	return 0;
}




int CVICALLBACK OnApply (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			med = 0;
			order = 0;
			if(filters != NULL){
				free(filters);
				filters = NULL;
			}	
			GraphSelection(panel, WAVE_PANEL_PREV,EVENT_COMMIT,0,0,0);
			break;
	}
	return 0;
}


// calculez si afisez derivata semnalului in domeniul timp. Aceasta o afisez peste valorile deja filtrate (indiferent de filtrul ales) cu o alta culoare
int CVICALLBACK OnDev (int panel, int control, int event, 
					   void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			if(selection == 2){
				dev = (double *) calloc(npoints, sizeof(double));
				Difference(filters, npoints, 1, 0, 0, dev);
				PlotY(panel, WAVE_PANEL_GRAPH_2, dev, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);  
				free(dev);	
			}else if(selection == 3){
				dev = (double *) calloc(sampleRate*(Stop - Start), sizeof(double));
				Difference(filterRes, sampleRate, 1, 0, 0, dev);
				PlotY(panel, WAVE_PANEL_GRAPH_2, dev, sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLACK);  
				free(dev);
			}
	}
	return 0;
}

// calcularea anvelopei si afisarea acesteia pe graficul 1. Totodata, am pregatit optiunea de a afisa aceeasi bucata/"pagina" concomitent.
// astfel, cand aceasta optiune este activata, utilizatorul va putea vizualiza semnalul initial, cu anvelopa calculata, impreuna cu un filtru ales
// peste care am suprapus si derivata acestuia in timp
int CVICALLBACK OnEnvelope (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			anvRes = anvelopa + (int)(Start * sampleRate);
			if(selection == 0 || selection == 2){
				PlotY(panel, WAVE_PANEL_GRAPH, anvelopa, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE); 
			}else if(selection == 1 || selection == 3){
				PlotY(panel, WAVE_PANEL_GRAPH, anvRes, sampleRate, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_BLUE);
			}
	}
	return 0;
}

int CVICALLBACK OnPanels (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2){
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == wavePanel){
				SetCtrlVal(frequencyPanel, FREQ_PANEL_SWITCHPANELS, 1);
				DisplayPanel(frequencyPanel);
				HidePanel(panel);
			}
			else{
				SetCtrlVal(wavePanel, WAVE_PANEL_SWITCHPANELS, 0);
				DisplayPanel(wavePanel);
				HidePanel(panel);
			}
			break;
	}
	return 0;
}


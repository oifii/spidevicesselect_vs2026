/*
 * Copyright (c) 2012-2026 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 1901 rue Gilford, #53
 * Montreal, QC, H2H 1G8
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
/** @file pa_devs.c
	@ingroup examples_src
    @brief List available devices, including device information.
	@author Phil Burk http://www.softsynth.com

    @note Define PA_USE_ASIO=0 to compile this code on Windows without
        ASIO support.
*/
/*
 * $Id: pa_devs.c 1752 2011-09-08 03:21:55Z philburk $
 *
 * This program uses the PortAudio Portable Audio Library.
 * For more information see: http://www.portaudio.com
 * Copyright (c) 1999-2000 Ross Bencina and Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however, 
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */

#include <stdio.h>
#include <math.h>
#include "portaudio.h"

 //2022oct11, spi, begin
/*
#ifdef WIN32
#if PA_USE_ASIO
#include "pa_asio.h"
#endif
#endif
*/
#if (defined(_WIN64) || defined(WIN32))
#if PA_USE_ASIO
#include "pa_asio.h"
#endif
#endif
#include "defs.h"
#include "spiaudiodevice.h"
//2022oct11, spi, end


#include <string>
#include <map>
using namespace std;
#include <windows.h>
#include <assert.h>

map<string,int> global_devicemap;


//2022oct11, spi, begin
int global_numchannels = NUM_CHANNELS;
SPIAudioDevice mySPIAudioDevice;
//2022oct11, spi, end


/*******************************************************************/
static void PrintSupportedStandardSampleRates(
        const PaStreamParameters *inputParameters,
        const PaStreamParameters *outputParameters )
{
    static double standardSampleRates[] = {
        8000.0, 9600.0, 11025.0, 12000.0, 16000.0, 22050.0, 24000.0, 32000.0,
        44100.0, 48000.0, 88200.0, 96000.0, 192000.0, -1 /* negative terminated  list */
    };
    int     i, printCount;
    PaError err;

    printCount = 0;
    for( i=0; standardSampleRates[i] > 0; i++ )
    {
        err = Pa_IsFormatSupported( inputParameters, outputParameters, standardSampleRates[i] );
        if( err == paFormatIsSupported )
        {
            if( printCount == 0 )
            {
                printf( "\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else if( printCount == 4 )
            {
                printf( ",\n\t%8.2f", standardSampleRates[i] );
                printCount = 1;
            }
            else
            {
                printf( ", %8.2f", standardSampleRates[i] );
                ++printCount;
            }
        }
    }
    if( !printCount )
        printf( "None\n" );
    else
        printf( "\n" );
}


///////////////////////////////////////////////////////////////
int main(int argc, char **argv);
int main(int argc, char **argv)
{
    int     i, numDevices, defaultDisplayed;
    const   PaDeviceInfo *deviceInfo;
    PaStreamParameters inputParameters, outputParameters;
    PaError err;

	/* for spi:
	"Microsoft Sound Mapper - Input" maps to 0
	"Wave (2- E-MU E-DSP Audio Proce" maps to 1
	"Microsoft Sound Mapper - Output" maps to 2
	"Speakers (2- E-MU E-DSP Audio P" maps to 3
	"Primary Sound Capture Driver" maps to 4
	"Wave (2- E-MU E-DSP Audio Processor (WDM))" maps to 5
	"Primary Sound Driver" maps to 6
	"Speakers (2- E-MU E-DSP Audio Processor (WDM))" maps to 7
	"ASIO4ALL v2" maps to 8
	"E-MU ASIO" maps to 9
	"E-DSP Wave [FFC0]" maps to 12
	*/
	//string selecteddevicename="E-MU ASIO";
	string selecteddevicename="";
	if(argc>1)
	{
		selecteddevicename = argv[1]; //for spi, device name could be "E-MU ASIO", 
	}
    
	//2022oct11, spi, begin
	string devicetxtfilename = "device.txt";
	FILE* pFILEdevice = fopen(devicetxtfilename.c_str(), "w");
	mySPIAudioDevice.m_pFILE = pFILEdevice;
	//2022oct11, spi, end


    Pa_Initialize();
	system("cls");

    printf( "PortAudio version number = %d\nPortAudio version text = '%s'\n",
            Pa_GetVersion(), Pa_GetVersionText() );


	//2022oct11, spi, begin
	/*

            
    numDevices = Pa_GetDeviceCount();
    if( numDevices < 0 )
    {
        printf( "ERROR: Pa_GetDeviceCount returned 0x%x\n", numDevices );
        err = numDevices;
        //goto error;
		assert(false);
    }
    
    printf( "Number of devices = %d\n", numDevices );
    for( i=0; i<numDevices; i++ )
    {
        deviceInfo = Pa_GetDeviceInfo( i );
		string devicenamestring = deviceInfo->name;
		global_devicemap.insert(pair<string,int>(devicenamestring,i));
	
        printf( "--------------------------------------- device #%d\n", i );
                
		//Mark global and API specific default devices
        defaultDisplayed = 0;
        if( i == Pa_GetDefaultInputDevice() )
        {
            printf( "[ Default Input" );
            defaultDisplayed = 1;
        }
        else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultInputDevice )
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( "[ Default %s Input", hostInfo->name );
            defaultDisplayed = 1;
        }
        
        if( i == Pa_GetDefaultOutputDevice() )
        {
            printf( (defaultDisplayed ? "," : "[") );
            printf( " Default Output" );
            defaultDisplayed = 1;
        }
        else if( i == Pa_GetHostApiInfo( deviceInfo->hostApi )->defaultOutputDevice )
        {
            const PaHostApiInfo *hostInfo = Pa_GetHostApiInfo( deviceInfo->hostApi );
            printf( (defaultDisplayed ? "," : "[") );                
            printf( " Default %s Output", hostInfo->name );
            defaultDisplayed = 1;
        }

        if( defaultDisplayed )
            printf( " ]\n" );



		//print device info fields
        printf( "Name                        = %s\n", deviceInfo->name );
        printf( "Host API                    = %s\n",  Pa_GetHostApiInfo( deviceInfo->hostApi )->name );
        printf( "Max inputs = %d", deviceInfo->maxInputChannels  );
        printf( ", Max outputs = %d\n", deviceInfo->maxOutputChannels  );

        printf( "Default low input latency   = %8.4f\n", deviceInfo->defaultLowInputLatency  );
        printf( "Default low output latency  = %8.4f\n", deviceInfo->defaultLowOutputLatency  );
        printf( "Default high input latency  = %8.4f\n", deviceInfo->defaultHighInputLatency  );
        printf( "Default high output latency = %8.4f\n", deviceInfo->defaultHighOutputLatency  );
//2022oct11, spi, begin
//#ifdef WIN32
#if (defined(_WIN64) || defined(_WIN32))
//2022oct11, spi, end
#if PA_USE_ASIO
//ASIO specific latency information
        if( Pa_GetHostApiInfo( deviceInfo->hostApi )->type == paASIO ){
            long minLatency, maxLatency, preferredLatency, granularity;

            err = PaAsio_GetAvailableLatencyValues( i,
		            &minLatency, &maxLatency, &preferredLatency, &granularity );

            printf( "ASIO minimum buffer size    = %ld\n", minLatency  );
            printf( "ASIO maximum buffer size    = %ld\n", maxLatency  );
            printf( "ASIO preferred buffer size  = %ld\n", preferredLatency  );

            if( granularity == -1 )
                printf( "ASIO buffer granularity     = power of 2\n" );
            else
                printf( "ASIO buffer granularity     = %ld\n", granularity  );
        }
#endif //PA_USE_ASIO 
//2022oct11, spi, begin
//#endif //WIN32 
#endif //(defined(_WIN64) || defined(_WIN32))
//2022oct11, spi, end

		if(0)
		{
			printf( "Default sample rate         = %8.2f\n", deviceInfo->defaultSampleRate );

			//poll for standard sample rates 
			inputParameters.device = i;
			inputParameters.channelCount = deviceInfo->maxInputChannels;
			inputParameters.sampleFormat = paInt16;
			inputParameters.suggestedLatency = 0; //ignored by Pa_IsFormatSupported() 
			inputParameters.hostApiSpecificStreamInfo = NULL;
        
			outputParameters.device = i;
			outputParameters.channelCount = deviceInfo->maxOutputChannels;
			outputParameters.sampleFormat = paInt16;
			outputParameters.suggestedLatency = 0; //ignored by Pa_IsFormatSupported() 
			outputParameters.hostApiSpecificStreamInfo = NULL;

			if( inputParameters.channelCount > 0 )
			{
				printf("Supported standard sample rates\n for half-duplex 16 bit %d channel input = \n",
						inputParameters.channelCount );
				PrintSupportedStandardSampleRates( &inputParameters, NULL );
			}

			if( outputParameters.channelCount > 0 )
			{
				printf("Supported standard sample rates\n for half-duplex 16 bit %d channel output = \n",
						outputParameters.channelCount );
				PrintSupportedStandardSampleRates( NULL, &outputParameters );
			}

			if( inputParameters.channelCount > 0 && outputParameters.channelCount > 0 )
			{
				printf("Supported standard sample rates\n for full-duplex 16 bit %d channel input, %d channel output = \n",
						inputParameters.channelCount, outputParameters.channelCount );
				PrintSupportedStandardSampleRates( &inputParameters, &outputParameters );
			}
		}
    }
    printf("----------------------------------------------\n\n");

	map<string,int>::iterator it;
	it = global_devicemap.find(selecteddevicename);
	if(it!=global_devicemap.end())
	{
		printf("%s maps to %d\n", selecteddevicename.c_str(), (*it).second);
	}
	else
	{
		for(it=global_devicemap.begin(); it!=global_devicemap.end(); it++)
		{
			printf("%s maps to %d\n", (*it).first.c_str(), (*it).second);
		}
	}

	*/
	mySPIAudioDevice.ScanAudioDevices();
	if (pFILEdevice) fclose(pFILEdevice);
	//2022oct11, spi, end

    Pa_Terminate();
    return 0;


error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return err;
}

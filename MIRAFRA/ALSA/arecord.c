#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<error.h>
#include<alsa/asoundlib.h>
#include<pthread.h>
#include<error.h>
#include<signal.h>
#include<fcntl.h>
#define PCM_DEVICE "default"
#define SAMPLE_RATE 48000    
#define CHANNELS 2          
#define SAMPLE_FORMAT SND_PCM_FORMAT_S16_LE 
#define BUFFER_SIZE 4096     
#define AUDIO_DURATION 120
#define FRAME_SIZE 32 //frame_size=bits_per_sample*channels 
short *capture_buffer;
short *write_buffer;
int PERIOD_SIZE=(BUFFER_SIZE)/(2*CHANNELS);  //buffer_size/(bytes_per_sample*no_of channels);
pthread_cond_t buffer_cond,write_complete_cond; 
pthread_mutex_t lock;
typedef struct {
	char riff[4];        // "RIFF"
	int size;            // Size of the WAV file
	char wave[4];        // "WAVE"
	char fmt[4];         // "fmt "
	int fmt_size;        // Format chunk size (16 for PCM)
	short audio_format;  // Audio format (1 = PCM)
	short num_channels;  // Number of channels (1 = Mono, 2 = Stereo)
	int sample_rate;     // Sample rate (e.g., 44100)
	int byte_rate;       // Byte rate = sample_rate * num_channels * bits_per_sample / 8
	short block_align;   // Block align (num_channels * bits_per_sample / 8)
	short bits_per_sample;// Bits per sample (e.g., 16)
	char data[4];        // "data"
	int data_size;       // Size of the audio data
} WavHeader;

///capture thread audio data
snd_pcm_t *pcm;
void * capture_audio(void *args)
{
	int err;
	while(1)
	{
		err=snd_pcm_readi(pcm,capture_buffer,PERIOD_SIZE);
		if(err<0)
		{
			if(err==-EPIPE)
			{
				printf("overrun occurred,resetting capture\n");
				snd_pcm_prepare(pcm);
			}
			else
			{
                        	free(capture_buffer);
	                        free(write_buffer);
				break;
			}
		}
		else
		{
			memcpy(write_buffer, capture_buffer,BUFFER_SIZE);
			pthread_cond_signal(&buffer_cond);
			pthread_cond_wait(&write_complete_cond,&lock);
		}

	}
}
void *write_audio(void *args)
{
	int record_file=*(int *)(args);
	int err;
	while(1)
	{
		pthread_cond_wait(&buffer_cond, &lock);
		int bytes=write(record_file,write_buffer,BUFFER_SIZE);
		pthread_cond_signal(&write_complete_cond);
	}

}
void write_wav_header(int file, int data_size) {
	WavHeader header = {
		.riff = {'R', 'I', 'F', 'F'},
		.size = 36 + data_size,
		.wave = {'W', 'A', 'V', 'E'},
		.fmt = {'f', 'm', 't', ' '},
		.fmt_size = 16,
		.audio_format = 1,
		.num_channels = CHANNELS,
		.sample_rate = SAMPLE_RATE,
		.byte_rate = SAMPLE_RATE * CHANNELS *( 16 / 8),
		.block_align = CHANNELS *( 16 / 8),
		.bits_per_sample = 16,
		.data = {'d', 'a', 't', 'a'},
		.data_size = data_size,
	};

	write(file,&header,sizeof(header));
}
void alarm_handler(int signum) {
		printf("recording completed for give time\n");
		snd_pcm_close(pcm);
		exit(0); // Terminate the program
	

}
void sigint_handler(int signum)
{
	printf("recording is finished\n");
	snd_pcm_close(pcm);
	exit(0);
}



int main(int argc,char **argv)
{
	if(argc<1)
	{
		printf("error message use:correct parameters\n");
		return 0;
	}
	int time=atoi(argv[1]);//time duration from command line 
	signal(SIGALRM, alarm_handler);//signal alaram stops recording after given duration
	signal(SIGINT,sigint_handler);
	alarm(time);//alarm(time): Triggers SIGALRM after time seconds.
	snd_pcm_hw_params_t *param;//ALSA PARAMETERS
	int err,rate,periods;
	rate=SAMPLE_RATE;
	periods=BUFFER_SIZE/PERIOD_SIZE;//NO OF PERIODS(CHUNKS) in the buffer
	capture_buffer = (short *)malloc(BUFFER_SIZE*2);
	write_buffer = (short *)malloc(BUFFER_SIZE*2);
	if (!capture_buffer || !write_buffer) {
		fprintf(stderr, "Error allocating memory\n");
		snd_pcm_close(pcm);
		return 1;
	}

	err=snd_pcm_open(&pcm,PCM_DEVICE,SND_PCM_STREAM_CAPTURE,0);//Opens ALSA's default PCM capture device (microphone input).
	if(err<0)
	{
		perror("snd_pcm_open");

		printf("error in file opening\n");
		exit(EXIT_FAILURE);

	}
	snd_pcm_hw_params_alloca(&param);//	Allocates memory for param on the stack.
	err=snd_pcm_hw_params_any(pcm,param);//Initializes param with default values for current device.
	if(err<0)
	{
		printf("error in configuring the hardware parameters\n");
		exit(EXIT_FAILURE);
	}
	err=snd_pcm_hw_params_set_access(pcm,param,SND_PCM_ACCESS_RW_INTERLEAVED);
	if(err<0)
	{
		printf("error in setting interleaved or non-interleaved\n");
		exit(EXIT_FAILURE);
	}


	err=snd_pcm_hw_params_set_rate(pcm,param,SAMPLE_RATE,0);
	/*SAMPLE_RATE	48000 samples/sec (CD quality).
	0 indicates Exact rate required. If device doesn’t support 48000, this will fail.*/
	if(err<0)
	{
		printf("error in setting sample rate\n");
		exit(EXIT_FAILURE);
	}

	err=snd_pcm_hw_params_set_rate_near(pcm,param,&rate,0);//set_rate_near() lets ALSA pick the nearest supported rate.
	if(err<0)
	{
		printf("error in setting near sample rate\n");
		exit(EXIT_FAILURE);
	}

	err=snd_pcm_hw_params_set_format(pcm,param,SAMPLE_FORMAT);//SND_PCM_FORMAT_S16_LE 16-bit signed PCM, Little Endian
	if(err<0)
	{
		printf("error in setting sample format\n");
		exit(EXIT_FAILURE);
	}
	err=snd_pcm_hw_params_set_channels(pcm,param,CHANNELS);//Stereo (Left + Right). Use 1 for Mono.
	if(err<0)
	{
		printf("error in setting the channels\n");
		exit(EXIT_FAILURE);
	}

	err = snd_pcm_hw_params_set_buffer_size(pcm, param, BUFFER_SIZE);//4096 bytesTotal size of audio buffer used for captur.                                                                         //The buffer will hold multiple periods.
	if(err<0)
	{
		printf("error in setting the buffer size\n");
		exit(EXIT_FAILURE);
	}

	err=snd_pcm_hw_params_set_period_size(pcm,param,PERIOD_SIZE,0);
	if(err<0)
	{

		printf("error in setting the perid size\n");
		perror("snd_pcm_hw_params_set_periods");
		exit(EXIT_FAILURE);
	}
	err=snd_pcm_hw_params_set_periods(pcm,param,periods,0);//This defines how many chunks the buffer will be divided into.
								//More periods = smoother playback/capture, but more latency.
	if(err<0)
	{
		perror("snd_pcm_hw_params_set_periods");
		exit(EXIT_FAILURE);
	}
	err=snd_pcm_hw_params(pcm,param);//This final call applies all the above settings to the PCM device.
	if(err<0)
	{
		printf("error in configuring hardware parameters\n");
		exit(EXIT_FAILURE);
	}        
	int record_file;
	record_file=open("file2.wav",O_CREAT|O_RDWR|O_TRUNC,0666);
	if(record_file<0)
	{
		perror("open");
		snd_pcm_close(pcm);
		exit(EXIT_FAILURE);
	}
	err=snd_pcm_prepare(pcm);//This prepares the PCM device for use after setting all hardware parameters.
	if(err<0)
	{
		printf("failed to prepare a PCM  device\n");
		snd_pcm_close(pcm);
		close(record_file);
		exit(EXIT_FAILURE);
	}

	int data_size =(AUDIO_DURATION*SAMPLE_RATE*CHANNELS)*(16/8);//total audio samples/bytes_per_sample

	write_wav_header(record_file,data_size);

	pthread_t capture_thread,write_thread;
	pthread_create(&capture_thread,NULL,capture_audio,NULL);//Reads audio data from the ALSA PCM device into a shared buffer
	pthread_create(&write_thread,NULL,write_audio,&record_file);
	//Takes the captured audio data from the buffer and writes it to disk

	printf("audio Recording started ......Press ctrl+c to stop recording \n");

	pthread_join(capture_thread,NULL);
	pthread_join(write_thread,NULL);

	snd_pcm_close(pcm);
	close(record_file);
	return 0;
}


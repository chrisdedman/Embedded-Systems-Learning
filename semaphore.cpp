#include <pthread.h>
#include <semaphore.h>
#include "GPIO.h"

static pthread_mutex_t button_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock1        = PTHREAD_MUTEX_INITIALIZER;

GPIO redLed(23);
GPIO yellowLed(24);
GPIO button(15);
GPIO button2(16);
GPIO button3(6);

sem_t semaphore1;

void *althernateBlink(void *value)
{
  pthread_mutex_lock(&lock1);
  printf("Alternating function Locked.\n");

  GPIO red(23), yellow(24);
  red.writeDirection("out");
  yellow.writeDirection("out");

  int openedLED  = red.openGPIOValue();
  int openedLED2 = yellow.openGPIOValue();

  sem_wait(&semaphore1);
  printf("Start alternating blinking.\n");

  for(int i = 0; i < 4; i++)
  {
    red.turnOnLED();
    sleep(1);
    red.turnOffLED();
    sleep(1);

    yellow.turnOnLED();
    sleep(1);
    yellow.turnOffLED();
    sleep(1);
  }

  pthread_mutex_unlock(&lock1);
  return NULL;
}

void yellowLEDShow(GPIO &yellow)
{
  sem_post(&semaphore1);

  for(int i = 0; i < 7; i++)
  {
    if(2 == i)
    {
      sleep(1);
    }

    if(4 == i) 
    {
      sleep(1);
    }

    yellow.turnOnLED();
    sleep(1);

    yellow.turnOffLED();
    sleep(1);
  }
}

void redLEDShow(GPIO &red)
{
  sem_post(&semaphore1);

  for(int i = 0; i < 5; i++)
  {
    if(3 == i)
    {
      sleep(1);
    }

    red.turnOnLED();
    sleep(1);

    red.turnOffLED();
    sleep(1);
  }
}

void interactWithLED(GPIO &led)
{
  led.writeDirection("out");
  int openedLED = led.openGPIOValue();

  if (1 == led.getLEDValue())
  {
    led.turnOffLED();
    led.setLEDValue(0);
    printf("LED Off!\n");
  }

  else
  {
    led.turnOnLED();
    led.setLEDValue(1);
    printf("LED On!\n");
  }

  close(openedLED);
}

void *pushButtonThread(void *value)
{ 
  pthread_mutex_lock(&button_mutex);
  button.setEdgeGPIO("rising");
  button2.setEdgeGPIO("rising");
  button3.setEdgeGPIO("rising");

  struct pollfd fdset[3];
  fdset[0].fd      = button.openGPIOValue();
  fdset[0].events  = POLLPRI;
  fdset[0].revents = 0;

  fdset[1].fd      = button2.openGPIOValue();
  fdset[1].events  = POLLPRI;
  fdset[1].revents = 0;

  fdset[2].fd      = button3.openGPIOValue();
  fdset[2].events  = POLLPRI;
  fdset[2].revents = 0;

  char buf[4];
  
  while(true)
  {
    read(fdset[0].fd, buf, 4);
    read(fdset[1].fd, buf, 4);
    read(fdset[2].fd, buf, 4);

    int rc = poll(fdset, 3, 10000);
    if (0 > rc)
    {
      printf("\npoll() failed!\n");
    }

    else if (0 == rc)
    {
      printf(".");
    }

    else
    {
      if (fdset[0].revents & POLLPRI)
      {
        interactWithLED(redLed);
      }

      if (fdset[1].revents & POLLPRI)
      {
        interactWithLED(yellowLed);
      }

      if (fdset[2].revents & POLLPRI)
      {
        redLed.writeDirection("out");
        int openRedLED = redLed.openGPIOValue();

        printf("\nStart Red Light Show!\n");
        redLEDShow(redLed);
        printf("\nEnd Red Light Show!\n");

        close(openRedLED);
      }
    }

    fflush(stdout);
  }

  pthread_mutex_unlock(&button_mutex);
  return NULL;
}

void *lightShowLED(void *value)
{
  GPIO redLed(23), yellowLed(24);
  redLed.setLEDValue(0);
  yellowLed.setLEDValue(0);

  GPIO button(6);
  button.setEdgeGPIO("rising");

  struct pollfd fdset[1];
  fdset[0].fd      = button.openGPIOValue();
  fdset[0].events  = POLLPRI;
  fdset[0].revents = 0;

  char buf[4];
  while(true)
  {
    read(fdset[0].fd, buf, 4);
    int rc = poll(fdset, 1, 10000);

    if (0 > rc)
    {
      printf("\npoll() failed!\n");
    }

    else if (0 == rc)
    {
      printf(".");
    }

    else
    {
      if (fdset[0].revents & POLLPRI)
      {
        yellowLed.writeDirection("out");
        int openYellowLED = yellowLed.openGPIOValue();

        printf("\nStart Yellow Light Show!\n");
        yellowLEDShow(yellowLed);
        printf("\nEnd Light Yellow Show!\n");

        close(openYellowLED);
      }
    }

    fflush(stdout);
  }

  return NULL;
}

int main()
{
  pthread_t thread[4];
  redLed.setLEDValue(0);
  yellowLed.setLEDValue(0);

  sem_init(&semaphore1, 1, 2);

  if (0 != pthread_mutex_init(&button_mutex, NULL))
  {
      printf("\n Mutex init failed\n");
      return 1;
  }
  
  if (0 != pthread_mutex_init(&lock1, NULL))
  {
      printf("\n Mutex init failed\n");
      return 1;
  }

  pthread_create(&thread[0], NULL, &pushButtonThread, NULL);
  pthread_create(&thread[1], NULL, &lightShowLED, NULL);
  pthread_create(&thread[2], NULL, &althernateBlink, NULL);

  while (true)
  {
    sleep(1);
  }

  pthread_mutex_destroy(&button_mutex);
  return 0;
}

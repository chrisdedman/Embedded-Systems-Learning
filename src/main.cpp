#include <pthread.h>
#include <semaphore.h>
#include <fstream>
#include <poll.h>
#include <unistd.h>
#include "GPIO.h"

static pthread_mutex_t button_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock1        = PTHREAD_MUTEX_INITIALIZER;
struct pollfd fdset[3];

GPIO redLed(23);
GPIO yellowLed(24);
GPIO button(15);
GPIO button2(16);
GPIO button3(6);

sem_t semaphore1;
sem_t semaphore2;

/*
 * Alternate Blinking LED Show
 * Alternate blinking red and yellow LED 4 times
 */
void *althernateBlink(void *value)
{
  pthread_mutex_lock(&lock1);
  printf("Alternating function Locked.\n");

  while (true)
  {
    sem_wait(&semaphore1);
    int openedLEDRed    = redLed.openGPIOValue();
    int openedLEDYellow = yellowLed.openGPIOValue();

    printf("Start alternating blinking.\n");
    for (int i = 0; i < 4; i++)
    {
      redLed.turnOnLED();
      sleep(1);
      redLed.turnOffLED();

      yellowLed.turnOnLED();
      sleep(1);
      yellowLed.turnOffLED();
    }

    close(openedLEDRed);
    close(openedLEDYellow);
  }

  pthread_mutex_unlock(&lock1);
  return NULL;
}

/*
 * YELLOW LED Show.
 * Blinks twice, pause one second, blink twice, pause 1 second blinks 3 times
 */
void yellowLEDShow(GPIO &yellow)
{
  yellow.writeDirection("out");
  int openYellowLED = yellow.openGPIOValue();

  for (int i = 0; i < 7; i++)
  {
    if (2 == i)
    {
      sleep(1);
    }

    if (4 == i)
    {
      sleep(1);
    }

    yellow.turnOnLED();
    sleep(1);
    yellow.turnOffLED();
    sleep(1);
  }

  close(openYellowLED);
}

/*
 * RED LED Show.
 * Blinks 3 times pause for 1 second blink twice
 */
void redLEDShow(GPIO &red)
{
  red.writeDirection("out");
  int openRedLED = red.openGPIOValue();

  for (int i = 0; i < 5; i++)
  {
    if (3 == i)
    {
      sleep(1);
    }

    red.turnOnLED();
    sleep(1);
    red.turnOffLED();
    sleep(1);
  }

  close(openRedLED);
}

/*
 * Turn On or Off an LED
 *
 * Parameters has the correcesponding GPIO pin
 */
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

/*
 * Initialize the poll with the buttons.
 */
void initializeFileDescriptorPoll()
{
  fdset[0].fd       = button.openGPIOValue();
  fdset[0].events   = POLLPRI;
  fdset[0].revents  = 0;

  fdset[1].fd       = button2.openGPIOValue();
  fdset[1].events   = POLLPRI;
  fdset[1].revents  = 0;

  fdset[2].fd       = button3.openGPIOValue();
  fdset[2].events   = POLLPRI;
  fdset[2].revents  = 0;
}

/*
 * Create a poll of event that will trigger
 * multiple action from the push buttons thread.
 *
 * First element of the poll will trigger a RED LED on/off
 * Second element of the poll will trigger a YELLOW LED on/off
 * Third element of the poll will trigger a light show.
 */
void *pushButtonThread(void *value)
{
  pthread_mutex_lock(&button_mutex);
  button.setEdgeGPIO("rising");
  button2.setEdgeGPIO("rising");
  button3.setEdgeGPIO("rising");

  // buffer to read FD poll
  char buf[4];

  while (true)
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
        printf("\nStart Red Light Show!\n");
        sem_post(&semaphore2);
        redLEDShow(redLed);
        printf("End Red Light Show!\n");
      }
    }

    fflush(stdout);
  }

  pthread_mutex_unlock(&button_mutex);
  return NULL;
}

/*
 * Start the Yellow LED Show when the third
 * button get a raising voltage.
 *
 * Use semaphore sem_post at the end of the LED Show
 * to start a alternating blinking LED show.
 */
void *lightShowLED(void *value)
{
  while (true)
  {
    sem_wait(&semaphore2);

    printf("\nStart Yellow Light Show!\n");
    yellowLEDShow(yellowLed);
    printf("End Light Yellow Show!\n");

    sem_post(&semaphore1);
    printf("Semaphore posted!\n");
  }

  return NULL;
}

int main()
{
  pthread_t thread[4];
  redLed.setLEDValue(0);
  yellowLed.setLEDValue(0);

  sem_init(&semaphore1, 0, 0);
  sem_init(&semaphore2, 0, 0);

  if (0 != pthread_mutex_init(&button_mutex, NULL))
  {
    printf("\n Mutex init failed!\n");
    return 1;
  }

  if (0 != pthread_mutex_init(&lock1, NULL))
  {
    printf("\n Mutex init failed!\n");
    return 1;
  }

  initializeFileDescriptorPoll();

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

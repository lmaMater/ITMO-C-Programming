#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpers.h"

void outErrors(char c) {
    char line[BUF_SIZE];
    char *str;
    FILE *input = fopen("access_log.txt", "r");
    str = fgets(line, BUF_SIZE, input);
    while (str != NULL) {
        int flag = 0;
        for (int i = 0; i < BUF_SIZE; i++) {
            if (str[i] == '\"' && flag == 1) {
                if (str[i + 2] == c) {
                    printf("%s", str);
                    break;
                } else {
                    break;
                }
            } else if (str[i] == '\"' && flag == 0) {
                flag += 1;
            }
        }
        str = fgets(line, BUF_SIZE, input);
    }
    fclose(input);
}

int main() {
    // out lines with 5xx - error
    outErrors('5');

    // out defined window with maximum requests
    int dayTimeWindow, hourTimeWindow, minTimeWindow, secTimeWindow;
    printf("\nEnter the number of days hours minutes seconds\n"
           "of the time window, separated by a space:\n");
    scanf("%d %d %d %d", &dayTimeWindow, &hourTimeWindow, &minTimeWindow, &secTimeWindow);

    int timeWindow = secTimeWindow + minTimeWindow * 60
                     + hourTimeWindow * 3600 + dayTimeWindow * 86400;
    char line[BUF_SIZE];
    char *str;

    FILE *input = fopen("access_log.txt", "r");
    str = fgets(line, BUF_SIZE, input);
    int curSeconds = stringToSeconds(str);
    int firstTimeMaxCount = 0, lastTimeMaxCount = 0, maxCount = 0,
            countElementInArr = -1, arrLength = BUF_SIZE;
    int count = 0, firstTime = curSeconds,
            lastTime = curSeconds, indexFirstElementInArr = 0;
    int *arr = (int *) malloc(arrLength * sizeof(int));
    while (str != NULL) {
        if (countElementInArr + 1 >= arrLength - 1) {
            arrLength *= 2;
            arr = realloc(arr, arrLength * sizeof(int));
        }
        if (curSeconds - firstTime <= timeWindow) {
            countElementInArr += 1;
            count += 1;
            arr[countElementInArr] = curSeconds;
            lastTime = curSeconds;
        } else {
            if (count > maxCount) {
                maxCount = count;
                firstTimeMaxCount = firstTime;
                lastTimeMaxCount = lastTime;
            }
            if (count > 1) {
                count -= 1;
                indexFirstElementInArr += 1;
                firstTime = arr[indexFirstElementInArr];
                while (curSeconds - firstTime > timeWindow && count > 1) {
                    count -= 1;
                    indexFirstElementInArr += 1;
                    firstTime = arr[indexFirstElementInArr];
                }
                if (curSeconds - firstTime <= timeWindow && count > 0) {
                    count += 1;
                    lastTime = curSeconds;
                    countElementInArr += 1;
                    arr[countElementInArr] = curSeconds;
                } else if (curSeconds - firstTime > timeWindow && count == 1) {
                    indexFirstElementInArr += 1;
                    firstTime = curSeconds;
                    lastTime = curSeconds;
                    countElementInArr += 1;
                    arr[countElementInArr] = curSeconds;
                }
            } else {
                indexFirstElementInArr += 1;
                firstTime = curSeconds;
                lastTime = curSeconds;
                countElementInArr += 1;
                arr[countElementInArr] = curSeconds;
            }

        }

        str = fgets(line, BUF_SIZE, input);
        if (str != NULL) {
            curSeconds = stringToSeconds(str);
        } else {
            if (count > maxCount) {
                maxCount = count;
                firstTimeMaxCount = firstTime;
                lastTimeMaxCount = lastTime;
            }
            break;
        }
    }

    printf("Maximum request number: %d\n", maxCount);

    int dayLast, hourLast, minLast, secLast;
    int dayFirst, hourFirst, minFirst, secFirst;

    secFirst = firstTimeMaxCount % 60;
    firstTimeMaxCount /= 60;
    minFirst = firstTimeMaxCount % 60;
    firstTimeMaxCount /= 60;
    hourFirst = firstTimeMaxCount % 60;
    firstTimeMaxCount /= 24;
    dayFirst = firstTimeMaxCount % 24;
    printf("First Time: 1995 %d Jul %d:%d:%d\n",
           dayFirst, hourFirst % 24,
           minFirst % 60, secFirst % 60);

    secLast = lastTimeMaxCount % 60;
    lastTimeMaxCount /= 60;
    minLast = lastTimeMaxCount % 60;
    lastTimeMaxCount /= 60;
    hourLast = lastTimeMaxCount % 60;
    lastTimeMaxCount /= 24;
    dayLast = lastTimeMaxCount % 24;
    printf("Last Time: 1995 %d Jul %d:%d:%d\n",
           dayLast, hourLast % 24,
           minLast % 60, secLast % 60);

    free(arr);

    return 0;
}

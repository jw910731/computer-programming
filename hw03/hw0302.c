#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linenoise.h>

#include "basic.h"
#include "file_selector.h"

enum result{
    HWIN,
    AWIN,
    DRAW,
};

struct teaminfo_t {
    i32 goal, against;
    i32 red;
    i32 points;
    i32 teamNo;
    i32 hwin, awin;
    i32 draw, lose;
};

struct matchinfo_t {
    // Date cannot exceed 20 character
    char date[20];
    i32 home, away;
    i32 hGoal, aGoal;
    i32 hShot, aShot;
    i32 hRed, aRed;
    enum result result;
};

const char *menu_msg = "1) Who is the winner in this season?\n\
2) Which team gets the most scores?\n\
3) Which team gets the most red cards?\n\
4) Which team wins the most games at home?\n\
5) Which team wins the most games away from home?\n\
6) Which game has the most scoring gap?\n\
7) Team information.\n\
8) Exit\n";

// global team record
static char **team_record = NULL;
static i32 len = 0, cap = 0;

int teamCmp(const void *a, const void *b);

static struct matchinfo_t *csvLoader(FILE *stream, i32 lineCount);

i32 team_num(const char *name);

i32 newline_counter(FILE *f);

int main() {
    linenoiseSetCompletionCallback(file_completion);
    char *s = linenoise("Please open a season record: ");
    FILE *csvFile = fopen(s, "r");
    if(csvFile == NULL) {
        perror("File open error");
        return 1;
    }

    // Loading and parsing CSV file
    i32 matchNum = newline_counter(csvFile) - 1;
    struct matchinfo_t *rows = csvLoader(csvFile, matchNum);
    struct teaminfo_t teams[len], sortedTeam[len];
    memset(&teams, 0x00, sizeof(struct teaminfo_t)*len);

    // summary team from match data
    for(i32 i = 0 ; i < matchNum ; ++i){
        // summary goal
        teams[rows[i].home].goal += rows[i].hGoal;
        teams[rows[i].away].goal += rows[i].aGoal;
        // summary red card
        teams[rows[i].home].red += rows[i].hRed;
        teams[rows[i].away].red += rows[i].aRed;
        // fill in team no
        teams[rows[i].home].teamNo = rows[i].home;
        teams[rows[i].away].teamNo = rows[i].away;
        // record against
        teams[rows[i].home].against += rows[i].aGoal;
        teams[rows[i].away].against += rows[i].hGoal;
        // calculate point
        switch(rows[i].result){
            case HWIN:
                teams[rows[i].home].points += 3;
                teams[rows[i].home].hwin++;
                teams[rows[i].away].lose++;
                break;
            case AWIN:
                teams[rows[i].away].points += 3;
                teams[rows[i].home].lose++;
                teams[rows[i].away].awin++;
                break;
            case DRAW:
                teams[rows[i].home].points += 1;
                teams[rows[i].away].points += 1;
                teams[rows[i].home].draw++;
                teams[rows[i].away].draw++;
                break;
        }
    }

    // available for query
    puts(menu_msg);
    bool endFlag = true, sorted = false;
    while(endFlag){
        i32 option = -1, ret = -2;
        printf("Choice (1-7, 8:exit): ");
        ret = scanf("%d", &option);
        if(ret != 1){
            continue;
        }
        // do option thing
        switch(option){
            case 1:
                if(!sorted){
                    memcpy(sortedTeam, teams, sizeof(struct teaminfo_t)*len);
                    qsort(sortedTeam, len, sizeof(struct teaminfo_t), teamCmp);
                    sorted = true;
                }
                printf("The winner is %s.\n", team_record[sortedTeam[0].teamNo]);
                break;
            case 2:{
                i32 mxsc = -1, mxteam = -1;
                for(i32 i = 0 ; i < len ; ++i){
                    if(mxsc < teams[i].goal){
                        mxsc = teams[i].goal;
                        mxteam = teams[i].teamNo;
                    }
                }
                printf("%s , %d\n", team_record[mxteam], mxsc);
                break;
            }
            case 3:{
                i32 mxsc = -1, mxteam = -1;
                for(i32 i = 0 ; i < len ; ++i){
                    if(mxsc < teams[i].red){
                        mxsc = teams[i].red;
                        mxteam = teams[i].teamNo;
                    }
                }
                printf("%s , %d\n", team_record[mxteam], mxsc);
                break;
            }
            case 4:{
                i32 mxsc = -1, mxteam = -1;
                for(i32 i = 0 ; i < len ; ++i){
                    if(mxsc < teams[i].hwin){
                        mxsc = teams[i].hwin;
                        mxteam = teams[i].teamNo;
                    }
                }
                printf("%s , %d\n", team_record[mxteam], mxsc);
                break;
            }
            case 5:{
                i32 mxsc = -1, mxteam = -1;
                for(i32 i = 0 ; i < len ; ++i){
                    if(mxsc < teams[i].awin){
                        mxsc = teams[i].awin;
                        mxteam = teams[i].teamNo;
                    }
                }
                printf("%s , %d\n", team_record[mxteam], mxsc);
                break;
            }
            case 6:{
                i32 mxGap = -1, mxMatch = -1;
                for(i32 i = 0 ; i < matchNum ; ++i){
                    i32 tmp = abs(rows[i].hGoal - rows[i].aGoal);
                    if(mxGap < tmp){
                        mxGap = tmp;
                        mxMatch = i;
                    }
                }
                printf("%s,%s(%d) vs %s(%d)\n", rows[mxMatch].date, team_record[rows[mxMatch].home], rows[mxMatch].hGoal, team_record[rows[mxMatch].away], rows[mxMatch].aGoal);
                break;
            }
            case 7:{
                printf("Team: ");
                static char buf[1024];
                while(getchar() != '\n'); // clear input buffer
                fgets_n(buf, 1024, stdin);
                i32 tm = -1;
                for(i32 i = 0 ; i < len && tm < 0 ; ++i) {
                    if(strcmp(team_record[i], buf) == 0){
                        tm = i;
                    }
                }
                if(tm < 0){
                    puts("Team not found...");
                    break;
                }
                printf("Points: %d\n", teams[tm].points);
                printf("Win/Draw/Lose: %d/%d/%d\n", teams[tm].awin + teams[tm].hwin, teams[tm].draw, teams[tm].lose);
                printf("Goals Scored/Goals Against: %d/%d\n", teams[tm].goal, teams[tm].against);
                break;
            }
            case 8:
                endFlag = false;
                break;
            default:
                puts("Input Error");
                break;
        }
    }
    // graceful shutdown
    free(rows);
    free(s);
    fclose(csvFile);
    return 0;
}

int teamCmp(const void *_a, const void *_b){
    const struct teaminfo_t *a = (struct teaminfo_t *)_a, *b = (struct teaminfo_t *)_b;
    if(a->points != b->points) return (a->points < b->points)? 1 : -1;
    i32 acp = a->goal - a->against, bcp = b->goal - b->against;
    if(acp != bcp) return (acp > bcp)? 1 : -1;
    if(a->goal != b->goal) return (a->goal < b->goal)? 1 : -1;
    return 0;
}

i32 team_num(const char *name) {
    i32 ret = -1;
    for(i32 i = 0 ; i < len && ret < 0 ; ++i) {
        if(strcmp(team_record[i], name) == 0){
            ret = i;
        }
    }
    // found
    if(ret >= 0)
        return ret;
    // not found
    if(cap >= len){ // allocate new space if space is not enough
        team_record = realloc(team_record, (cap + 1) *2 * sizeof(char *));
        cap = (cap + 1) * 2;
    }
    // insert
    size_t l = strlen(name);
    team_record[len] = calloc(l+1, sizeof(char));
    strncpy(team_record[len], name, l+1);
    return len++;
}

static struct matchinfo_t *csvLoader(FILE *f, i32 matchNum) {
    struct matchinfo_t *ret = calloc(matchNum, sizeof(struct matchinfo_t));
    static char buf[2048];
    fgets(buf, 2048, f);
    for(i32 i = 0 ; i < matchNum ; ++i){
        fgets(buf, 2048, f);
        char *sep = strtok(buf, ",");
        i32 count = 0;
        while(sep != NULL){
            // date
            switch(count){
                case 0:
                    strncpy(ret[i].date, sep, 20);
                    break;
                case 1:
                    ret[i].home = team_num(sep);
                    break;
                case 2:
                    ret[i].away = team_num(sep);
                    break;
                case 3:
                    ret[i].hGoal = (i32)(strtol(sep, NULL, 10));
                    break;
                case 4:
                    ret[i].aGoal = (i32)(strtol(sep, NULL, 10));
                    break;
                case 5:{
                    enum result tmp;
                    switch(*sep){
                        case 'D':
                            tmp = DRAW;
                            break;
                        case 'A':
                            tmp = AWIN;
                            break;
                        case 'H':
                            tmp = HWIN;
                            break;
                        default:
                            fprintf(stderr, "Invalid csv format!");
                            exit(1);
                            break;
                    }
                    ret[i].result = tmp;
                    break;
                }
                case 10:
                    ret[i].hShot = (i32)(strtol(sep, NULL, 10));
                    break;
                case 11:
                    ret[i].aShot = (i32)(strtol(sep, NULL, 10));
                    break;
                case 20:
                    ret[i].hRed = (i32)(strtol(sep, NULL, 10));
                    break;
                case 21:
                    ret[i].aRed = (i32)(strtol(sep, NULL, 10));
                default:
                    break;
            }
            ++count;
            sep = strtok(NULL, ",");
        }
    }
    return ret;
}

i32 newline_counter(FILE *f){
    i64 pos = ftell(f);
    rewind(f);
    static char buf[1024];
    i32 cnt = 0;
    while(!feof(f)){
        i64 len = fread(buf, 1, 1024, f);
        for(char *it = buf; it - buf < len ; ++it){
            if(*it == '\n') ++cnt;
        }
    }
    fseek(f, pos, SEEK_SET);
    return cnt;
}

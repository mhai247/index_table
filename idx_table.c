#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ELEMENT 10000

typedef struct word_struct
{
    char word[50];
    int index;
} word_struct;

char *word_handle(char *word, int *punct)
{
    int i, gap = 0;
    int len = strlen(word);
    int start = 1;
    int next_punct = (word[len - 1] == '.' || word[len - 1] == '?' || word[len - 1] == '!');
    for (i = 0; i < len; ++i)
        if (isalpha(word[i]))
        {
            if (start && isupper(word[i]) && !(*punct))
            {
                *punct = next_punct;
                return "";
            }
            else
                start = 0;
            word[i - gap] = tolower(word[i]);
        }
        else
            gap++;

    word[len - gap] = '\0';
    *punct = next_punct;
    return word;
}

int get_idx(word_struct words[MAX_ELEMENT], char *word, int num_words)
{
    int i;
    for (i = 0; i < num_words; ++i)
        if (strcmp(words[i].word, word) == 0)
            return i;
    return -1;
}

int stp_w(char *word, char stopword[MAX_ELEMENT][50], int n_stop_word)
{
    int i;
    for (i = 0; i < n_stop_word; ++i)
        if (strcmp(stopword[i], word) == 0)
            return 1;
    return 0;
}

int main()
{
    char word[50], c, line[1000], *ptr;
    word_struct words[MAX_ELEMENT];
    int n_word = 0;
    char stop_word[MAX_ELEMENT][50];
    int line_idx = 1, punct = 1;
    int i = 0, n_stop_word = 0, j, m;
    int word_count[MAX_ELEMENT];
    int pos[MAX_ELEMENT][100];
    int line_count[MAX_ELEMENT];
    // return 0;

    FILE *fp = fopen("alice30.txt", "r"),
         *fp2 = fopen("stopw.txt", "r");
    while (fscanf(fp2, "%s", stop_word[n_stop_word]) == 1)
        n_stop_word++;

    while (!feof(fp))
    {
        if (fgets(line, sizeof(line), fp) != NULL)
        {
            ptr = line;
            while (ptr)
            {
                if (sscanf(ptr, "%s", word) != EOF)
                {
                    strcpy(word, word_handle(word, &punct));
                    // printf("%s\n", word);
                    if (strlen(word) != 0 && !stp_w(word, stop_word, n_stop_word))
                    {
                        i = get_idx(words, word, n_word);
                        if (i == -1)
                        {
                            strcpy(words[n_word].word, word);
                            words[n_word].index = n_word;
                            pos[n_word][0] = line_idx;
                            word_count[n_word] = 1;
                            line_count[n_word] = 1;
                            n_word++;
                        }
                        else
                        {
                            if (pos[i][line_count[i] - 1] != line_idx)
                            {
                                pos[i][line_count[i]] = line_idx;
                                line_count[i]++;
                            }
                            word_count[i]++;
                        }
                    }
                }
                ptr = strchr(ptr + 1, ' ');
                if (ptr)
                    ptr += 1;
            }
            line_idx++;
        }
    } // return 0;

    word_struct tmp;

    for (i = 0; i < n_word; ++i)
        for (j = n_word - 1; j >= i; --j)
            if (strcmp(words[j].word, words[j - 1].word) < 0)
            {
                tmp = words[j];
                words[j] = words[j - 1];
                words[j - 1] = tmp;
            }
    for (j = 0; j < n_word; ++j)
    {
        i = words[j].index;
        printf("%s %d", words[j].word, word_count[i]);
        for (m = 0; m < line_count[i]; ++m)
            printf(",%d", pos[i][m]);
        printf("\n");
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 256
#define LINE_LEN (NAME_LEN+7)

#define ALLOC_CHECK(p) if (!(p)) fputs("Neuspesna alokacija", stderr), exit(1)
#define FILE_CHECK(f)  if (!(f)) perror(NULL), exit(2)

typedef struct elem {
    char name[NAME_LEN];
    int min, sec;
    struct elem *next;
} Playlist;

int duration(Playlist *elem)
{
    return elem->min * 60 + elem->sec;
}

// Ubacuje u listu u rastucem poretku (** jer se glava moze promeniti)
void insert(Playlist **phead, Playlist *elem)
{
    Playlist *prev = NULL, *p = *phead;

    while (p && duration(p) <= duration(elem)) {
        prev = p;
        p = p->next;
    }

    if (prev) prev->next = elem;
    else      *phead = elem;
    elem->next = p;
}

Playlist *read_playlist(FILE *fin)
{
    Playlist *head = NULL;
    char line[LINE_LEN];

    while (fgets(line, LINE_LEN, fin)) {
        char *last_space = strrchr(line, ' ');
        if (!last_space) break;

        Playlist *p = calloc(1, sizeof *p); // name="", next=NULL
        ALLOC_CHECK(p);

        strncat(p->name, line, last_space-line); // Prepisuje ime do posl razmaka
        sscanf(last_space, "%d:%d", &p->min, &p->sec); // Izvlaci vreme

        insert(&head, p);
    }

    return head;
}

void free_playlist(Playlist *list)
{
    while (list) {
        Playlist *p = list;
        list = list->next;
        free(p);
    }
}

int main(int argc, char *argv[])
{
    FILE *fin, *fout;

    if (argc != 3) return 3;
    FILE_CHECK(fin  = fopen(argv[1], "r"));
    FILE_CHECK(fout = fopen(argv[2], "w"));

    Playlist *list = read_playlist(fin); // Ucita uredjeno rastuce po trajanju

    int length;
    printf("Uneti novu duzinu: ");
    scanf("%d", &length);

    // Posto je lista uredjena rastuce, ne treba ni praviti drugu listu, vec se
    // samo prolazi i ispisuju se elementi dok se ne predje zadata duzina
    Playlist *p = list;
    while (p && (length -= duration(p)) >= 0) {
        fprintf(fout, "%s %2d:%2d\n", p->name, p->min, p->sec);
        p = p->next;
    }

    free_playlist(list);
    fclose(fin);
    fclose(fout);
    return 0;
}
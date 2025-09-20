#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __WIN32
#include <ncurses/curses.h>
#else
#include <curses.h>
#endif

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Invalid argument\n");
        return 0;
    }
    FILE* file = fopen(argv[1], "r");
    
    fseek(file, 0, SEEK_END);
    int file_size = ftell(file);
    rewind(file);
    
    int buffer_size = file_size + 1;
    char* buffer = calloc(buffer_size, sizeof(char));
    int read_bytes = fread(buffer, sizeof(char), file_size, file);
    if (read_bytes != file_size) {
        printf("Failed to read file content\n");
        return -1;
    }

    fclose(file);

    int lines_size = 0;
    for (int i = 0; i < file_size; ++i) {
        if (buffer[i] == '\n') {
            ++lines_size;
        }
    }
    ++lines_size;

    char** lines = calloc(lines_size, sizeof(char*));
    lines[0] = buffer;
    for (int i = 0, j = 0; i < buffer_size; ++i) {
        if (buffer[i] == '\n') {
            lines[++j] = buffer + i + 1;
        }
    }

    int* lines_lens = calloc(lines_size, sizeof(int));
    for (int i = 1; i < lines_size; ++i) {
        lines_lens[i-1] = lines[i] - lines[i-1] - 1;
    }
    lines_lens[lines_size-1] = buffer_size - (int)(long long)lines[lines_size-1];
    
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();

    WINDOW* frame = newwin(LINES, COLS, 0, 0);
    box(frame, 0, 0);
    if (argv[1] != NULL) {
        mvwaddnstr(frame, 0, (COLS - strlen(argv[1])) / 2, argv[1], strlen(argv[1]));
    }
    wrefresh(frame);

    int text_display_width = COLS - 2;
    int text_display_height = LINES - 2;
    WINDOW* text_display = newwin(text_display_height, text_display_width, 1, 1);
    keypad(text_display, true);

    bool is_running = true;
    int current_line = 0;
    int current_column = 0;
    while (is_running) {
        werase(text_display);

        for (int i = 0; i < text_display_height; ++i) {
            if (current_line + i >= lines_size) {
                break;
            }
            if (current_column >= lines_lens[current_line + i]) {
                continue;
            }
            if (current_column < lines_lens[current_line + i])
                mvwaddnstr(text_display, i, 0, lines[current_line + i] + current_column, text_display_width);
        }

        int c = wgetch(text_display);
        switch (c) {
            case 'q':
                is_running = false;
                break;
            case KEY_DOWN:
                if (current_line + text_display_height < lines_size) {
                    ++current_line;
                }
                break;
            case KEY_UP:
                if (current_line - 1 >= 0) {
                    --current_line;
                }
                break;
            case KEY_RIGHT:
                ++current_column;
                break;
            case KEY_LEFT:
                if (current_column - 1 >= 0) {
                    --current_column;
                }
                break;
            case KEY_NPAGE:
                if (current_line + 2 * text_display_height < lines_size) {
                    current_line += text_display_height;
                } else {
                    current_line = lines_size - text_display_height;
                }
                break;
            case KEY_PPAGE:
                if (current_line - text_display_height >= 0) {
                    current_line -= text_display_height;
                } else {
                    current_line = 0;
                }
                break;
        }
        mvwhline(frame, LINES-1, 1, 0, COLS-2);
        // mvwaddstr(frame, LINES-1, 1, keyname(c));
        mvwprintw(frame, LINES-1, 1, "%s, l %d, c %d", keyname(c), current_line, current_column);
        wrefresh(frame);
    }

    free(lines);
    free(buffer);
    delwin(text_display);
    delwin(frame);
    endwin();
    return 0;
}
#include <sqlite3ext.h>
#include <stddef.h>
#include <string.h>
SQLITE_EXTENSION_INIT1

static void snippet_ext(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc == 4) {
        const char *columnValue = (const char *)sqlite3_value_text(argv[0]);
        const char *targetWord = (const char *)sqlite3_value_text(argv[1]);
        const char *startTag = (const char *)sqlite3_value_text(argv[2]);
        const char *endTag = (const char *)sqlite3_value_text(argv[3]);

        if (columnValue && targetWord && startTag && endTag) {
            size_t columnLen = strlen(columnValue);
            size_t targetWordLen = strlen(targetWord);
            size_t startTagLen = strlen(startTag);
            size_t endTagLen = strlen(endTag);

            // Calculate the length of the processed value
            size_t processedLen = 0;
            const char *currentPos = columnValue;
            while (*currentPos) {
                if (strncmp(currentPos, targetWord, targetWordLen) == 0) {
                    processedLen += startTagLen + targetWordLen + endTagLen;
                    currentPos += targetWordLen;
                } else {
                    processedLen++;
                    currentPos++;
                }
            }

            // Allocate memory for the processed value
            char *processedValue = sqlite3_malloc(processedLen + 1);
            if (processedValue) {
                // Process the column value and insert HTML snippet
                char *outputPos = processedValue;
                currentPos = columnValue;
                while (*currentPos) {
                    if (strncmp(currentPos, targetWord, targetWordLen) == 0) {
                        strcpy(outputPos, startTag);
                        outputPos += startTagLen;
                        strcpy(outputPos, targetWord);
                        outputPos += targetWordLen;
                        strcpy(outputPos, endTag);
                        outputPos += endTagLen;
                        currentPos += targetWordLen;
                    } else {
                        *outputPos++ = *currentPos++;
                    }
                }

                *outputPos = '\0';

                // Set the processed value as the result
                sqlite3_result_text(context, processedValue, processedLen, sqlite3_free);
            } else {
                sqlite3_result_error(context, "Memory allocation failed", -1);
            }
        } else {
            sqlite3_result_error(context, "Invalid arguments", -1);
        }
    } else {
        sqlite3_result_error(context, "Invalid number of arguments. Expected 4.", -1);
    }
}

int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi) {
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi)

    rc = sqlite3_create_function(
        db,
        "snippet_ext",
        4,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC,
        NULL,
        snippet_ext,
        NULL,
        NULL
    );

    return rc;
}

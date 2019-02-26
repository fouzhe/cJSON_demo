#include <stdio.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

char *create_monitor_with_helpers(void)
{
    const char *strings[2] = {"1","2"};
    char *string = NULL;
    cJSON *resolutions = NULL;
    size_t index = 0;

    cJSON *monitor = cJSON_CreateObject();

    if (cJSON_AddNumberToObject(monitor, "start", 1) == NULL)
    {
        goto end;
    }

    if (cJSON_AddNumberToObject(monitor, "end", 5) == NULL)
    {
        goto end;
    }

    resolutions = cJSON_CreateStringArray(strings, 2);
    if (resolutions == NULL)
    {
        goto end;
    }

    cJSON_AddItemToObject(monitor, "resolutions", resolutions);

    string = cJSON_Print(monitor);
    if (string == NULL) {
        fprintf(stderr, "Failed to print monitor.\n");
    }

end:
    cJSON_Delete(monitor);
    return string;
}

/* return 1 if the monitor supports full hd, 0 otherwise */
int supports_full_hd(const char * const monitor)
{
    const cJSON *resolutions = NULL;
    const cJSON *resolution = NULL;
    const cJSON *start = NULL;
    const cJSON *end = NULL;
    int status = 0;
    cJSON *monitor_json = cJSON_Parse(monitor);
    if (monitor_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        status = 0;
        goto end;
    }

    start = cJSON_GetObjectItemCaseSensitive(monitor_json, "start");
    if (cJSON_IsNumber(start))
    {
        printf(" start: %f\n", start->valuedouble);
    }

    end = cJSON_GetObjectItemCaseSensitive(monitor_json, "end");
    if (cJSON_IsNumber(start))
    {
        printf(" end: %f\n", end->valuedouble);
    }

    resolutions = cJSON_GetObjectItemCaseSensitive(monitor_json, "resolutions");

    int array_size = cJSON_GetArraySize(resolutions);
    printf(" array size: %d\n", array_size);
    int i = 0;
    for(i = 0; i < array_size; i ++ )
    {
        resolution = cJSON_GetArrayItem(resolutions, i);
        if(!cJSON_IsString(resolution))
        {
            status = 0;
            goto end;
        }
        printf(" %s\n", resolution->valuestring);
    }
    status = 1;
end:
    cJSON_Delete(monitor_json);
    return status;
}

static char *read_file(const char *filename)
{
    FILE *file = NULL;
    long length = 0;
    char *content = NULL;
    size_t read_chars = 0;

    /* open in read binary mode */
    file = fopen(filename, "rb");
    if (file == NULL)
    {
        goto cleanup;
    }

    /* get the length */
    if (fseek(file, 0, SEEK_END) != 0)
    {
        goto cleanup;
    }
    length = ftell(file);
    if (length < 0)
    {
        goto cleanup;
    }
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        goto cleanup;
    }

    /* allocate content buffer */
    content = (char*)malloc((size_t)length + sizeof(""));
    if (content == NULL)
    {
        goto cleanup;
    }

    /* read the file into memory */
    read_chars = fread(content, sizeof(char), (size_t)length, file);
    if ((long)read_chars != length)
    {
        free(content);
        content = NULL;
        goto cleanup;
    }
    content[read_chars] = '\0';


cleanup:
    if (file != NULL)
    {
        fclose(file);
    }

    return content;
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s *.json\n",argv[0]);
        return 0;
    }
    char *json = NULL;
    json = read_file(argv[1]);
    if(json != NULL)
    {
        supports_full_hd(json);
    }
    else 
    {
        fprintf(stderr, "Failed to parse.\n");
    }

    
}

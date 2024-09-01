#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define server "127.0.0.1"
#define user "root"
#define password "password"
#define database "demo2"
#define port 3306

void insert_data(MYSQL *conn, int id, char *name, int score1, int score2)
{
    char query[512];
    sprintf(query, "INSERT INTO table1 (id, name, score1, score2, changeTime) VALUES (%d, '%s', %d, %d, NOW())", id, name, score1, score2);

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Insert data error: %s\"}\n", mysql_error(conn));
    }
    else
    {
        printf("{\"message\": \"Insert data successfully.\"}\n");
    }
}

void delete_data(MYSQL *conn, int id)
{
    char query[128];
    sprintf(query, "DELETE FROM table1 WHERE id=%d", id);

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Delete data error: %s\"}\n", mysql_error(conn));
    }
    else
    {
        int affected_rows = mysql_affected_rows(conn);
        if (affected_rows == 0)
        {
            printf("{\"error\": \"No data found to delete.\"}\n");
        }
        else
        {
            printf("{\"message\": \"Delete data successfully.\", \"rowsAffected\": %d}\n", affected_rows);
        }
    }
}

void query_data(MYSQL *conn)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[128] = "SELECT * FROM table1";

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Query data error: %s\"}\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        printf("{\"error\": \"Fetch result error: %s\"}\n", mysql_error(conn));
        return;
    }

    printf("[");
    int first = 1;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        if (!first)
        {
            printf(",");
        }
        printf("{\"id\": \"%s\", \"name\": \"%s\", \"score1\": \"%s\", \"score2\": \"%s\", \"changeTime\": \"%s\"}", row[0], row[1], row[2], row[3], row[4]);
        first = 0;
    }
    printf("]\n");

    mysql_free_result(res);
}

void update_data(MYSQL *conn, int id, char *name, int score1, int score2)
{
    char query[512];
    sprintf(query, "SELECT COUNT(*) FROM table1 WHERE id=%d", id);
    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Check id existence error: %s\"}\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    int count = atoi(row[0]);
    mysql_free_result(res);

    if (count > 0)
    {
        sprintf(query, "UPDATE table1 SET name='%s', score1=%d, score2=%d, changeTime=NOW() WHERE id=%d", name, score1, score2, id);
        if (mysql_query(conn, query))
        {
            printf("{\"error\": \"Update data error: %s\"}\n", mysql_error(conn));
        }
        else
        {
            printf("{\"message\": \"Update data successfully.\"}\n");
        }
    }
    else
    {
        insert_data(conn, id, name, score1, score2);
    }
}

void sort_data_by_score(MYSQL *conn, char *score_column, char *order)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[256];
    sprintf(query, "SELECT * FROM table1 ORDER BY %s %s", score_column, (strcmp(order, "asc") == 0 ? "ASC" : "DESC"));

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Sort data error: %s\"}\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        printf("{\"error\": \"Fetch result error: %s\"}\n", mysql_error(conn));
        return;
    }

    printf("[");
    int first = 1;
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        if (!first)
        {
            printf(",");
        }
        printf("{\"id\": \"%s\", \"name\": \"%s\", \"score1\": \"%s\", \"score2\": \"%s\", \"changeTime\": \"%s\"}", row[0], row[1], row[2], row[3], row[4]);
        first = 0;
    }
    printf("]\n");

    mysql_free_result(res);
}

void find_student_by_id(MYSQL *conn, int id)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[256];
    sprintf(query, "SELECT * FROM table1 WHERE id=%d", id);

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Find student error: %s\"}\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        printf("{\"error\": \"Fetch result error: %s\"}\n", mysql_error(conn));
        return;
    }

    if ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("[{\"id\": \"%s\", \"name\": \"%s\", \"score1\": \"%s\", \"score2\": \"%s\", \"changeTime\": \"%s\"}]\n", row[0], row[1], row[2], row[3], row[4]);
    }
    else
    {
        printf("[]\n");
    }

    mysql_free_result(res);
}

void display_total_count(MYSQL *conn)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char query[128] = "SELECT COUNT(*) FROM table1";

    if (mysql_query(conn, query))
    {
        printf("{\"error\": \"Count data error: %s\"}\n", mysql_error(conn));
        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        printf("{\"error\": \"Fetch result error: %s\"}\n", mysql_error(conn));
        return;
    }

    row = mysql_fetch_row(res);
    printf("{\"count\": \"%s\"}\n", row[0]);

    mysql_free_result(res);
}

int main(int argc, char *argv[])
{
    MYSQL *mysql = mysql_init(NULL);
    if (mysql == NULL)
    {
        printf("{\"error\": \"mysql_init() error\"}\n");
        return -1;
    }

    mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    if (!mysql_real_connect(mysql, server, user, password, database, port, NULL, 0))
    {
        printf("{\"error\": \"mysql_real_connect() error: %s\"}\n", mysql_error(mysql));
        return -1;
    }

    if (mysql_set_character_set(mysql, "utf8mb4"))
    {
        printf("{\"error\": \"Failed to set character set to utf8mb4: %s\"}\n", mysql_error(mysql));
    }

    if (argc < 2)
    {
        printf("{\"error\": \"Usage: %s <operation> [params]\"}\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "insert") == 0 && argc == 6)
    {
        int id = atoi(argv[2]);
        char *name = argv[3];
        int score1 = atoi(argv[4]);
        int score2 = atoi(argv[5]);
        insert_data(mysql, id, name, score1, score2);
    }
    else if (strcmp(argv[1], "delete") == 0 && argc == 3)
    {
        int id = atoi(argv[2]);
        delete_data(mysql, id);
    }
    else if (strcmp(argv[1], "query") == 0 && argc == 2)
    {
        query_data(mysql);
    }
    else if (strcmp(argv[1], "update") == 0 && argc == 6)
    {
        int id = atoi(argv[2]);
        char *name = argv[3];
        int score1 = atoi(argv[4]);
        int score2 = atoi(argv[5]);
        update_data(mysql, id, name, score1, score2);
    }
    else if (strcmp(argv[1], "sort") == 0 && argc == 4)
    {
        char *score_column = argv[2];
        char *order = argv[3];
        sort_data_by_score(mysql, score_column, order);
    }
    else if (strcmp(argv[1], "find") == 0 && argc == 3)
    {
        int id = atoi(argv[2]);
        find_student_by_id(mysql, id);
    }
    else if (strcmp(argv[1], "count") == 0 && argc == 2)
    {
        display_total_count(mysql);
    }
    else
    {
        printf("{\"error\": \"Invalid command or insufficient parameters.\"}\n");
    }

    mysql_close(mysql);
    return 0;
}

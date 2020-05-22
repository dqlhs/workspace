#ifndef DATABASEDEFINE_H
#define DATABASEDEFINE_H
#define HS_LOCAL_DATABASE     "HS_LocalInfo.db"
//#define HS_DB_DIRECTORY       (STORAGE_FOLDER"\\"HS_LOCAL_DATABASE)
#define HS_MEMORY_DB_DIRECTORY (":memory:")

#define TABLE_CR_SPEC           "table_CR_"
#define TABLE_CR(MAC_ADDRESS)   (TABLE_CR_SPEC + MAC_ADDRESS)
#define TABLE_CALL_RECORD       "table_CallRecord"
#define COLUMN_CALL_RECORD_ID   "CallRecordId"
#define COLUMN_CALL_TIME        "CallTime"
#define COLUMN_CALLER_NAME      "CallerName"
#define COLUMN_CALL_NUMBER      "CallNumber"
#define COLUMN_CALL_DURATION    "CallDuration"
#define COLUMN_CALL_TYPE        "CallType"
#define COLUMN_CALL_TALKTIME     "CallTalktime"
#define TABLE_SQLITE_SEQUENCE   "sqlite_sequence"

#define TABLE_TEMP_CB_SPEC           "table_TEMP_CB_"
#define TABLE_CB_SPEC           "table_CB_"
#define TABLE_CB(MAC_ADDRESS)   (TABLE_CB_SPEC + MAC_ADDRESS)
#define TABLE_CONTACT_BOOK      "table_ContactBook"
#define COLUMN_CONTACT_ID       "ContactId"
#define COLUMN_CONTACT_NAME     "ContactName"
#define COLUMN_CONTACT_NUMBER   "ContactNumber"
#define COLUMN_CONTACT_TYPE     "ContactType"
#define COLUMN_CONTACT_PINYIN   "ContactPinYin"
#define COLUMN_CONTACT_FIRSTLETTERS "ContactFirstLetters"
#define COLUMN_CONTACT_FIRSTLETTER  "ContactFirstLetter"

#define CREATE_TABLE            "CREATE TABLE "
#define CREATE_TABLE_CALL_RECORD(TABLE_NAME) \
                                    (CREATE_TABLE + TABLE_NAME +  \
                                    " (" COLUMN_CALL_RECORD_ID " INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                         COLUMN_CALLER_NAME" TEXT, " \
                                         COLUMN_CALL_NUMBER" TEXT, " \
                                         COLUMN_CALL_TYPE" TEXT, " \
                                         COLUMN_CALL_TIME" TEXT, " \
                                         COLUMN_CALL_TALKTIME" TEXT );")

#define CREATE_TABLE_CONTACT_BOOK(TABLE_NAME) \
                                    (CREATE_TABLE + TABLE_NAME + \
                                    " (" COLUMN_CONTACT_ID " INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                        COLUMN_CONTACT_NAME" TEXT, " \
                                        COLUMN_CONTACT_NUMBER" TEXT, " \
                                        COLUMN_CONTACT_TYPE" INTEGER, " \
                                        COLUMN_CONTACT_FIRSTLETTERS" TEXT, " \
                                        COLUMN_CONTACT_FIRSTLETTER" TEXT );")
#define CREATE_TEMPTABLE_CONTACT_BOOK(TABLE_NAME) \
                                    (CREATE_TABLE + TABLE_NAME + \
                                    " (" COLUMN_CONTACT_ID " INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                        COLUMN_CONTACT_NAME" TEXT, " \
                                        COLUMN_CONTACT_NUMBER" TEXT, " \
                                        COLUMN_CONTACT_TYPE" INTEGER, " \
                                        COLUMN_CONTACT_FIRSTLETTERS" TEXT, " \
                                        COLUMN_CONTACT_FIRSTLETTER" TEXT );")


#define DROP_TABLE                  "DROP TABLE "
#define DROP_TABLE_CALL_RECORD(TABLE_NAME) \
                                    (DROP_TABLE + TABLE_NAME +";")
#define DROP_TABLE_CONTACT_BOOK(TABLE_NAME) \
                                    (DROP_TABLE + TABLE_NAME +";")

#define SELECT_TABLE                "SELECT "
#define EXIST_TABLE(TABLE_NAME) \
                                  (SELECT_TABLE"* FROM sqlite_master WHERE type='table' AND name = '" + TABLE_NAME +"';")
#define COUNT_CALLOGTABLE        (SELECT_TABLE"* FROM sqlite_master WHERE type='table' AND name LIKE  'table_CR%';")
#define COUNT_CONTACTTABLE        (SELECT_TABLE"* FROM sqlite_master WHERE type='table' AND name LIKE  'table_CB%';")
#define COUNT_TABLE               (SELECT_TABLE"* FROM sqlite_master WHERE type='table' AND name !='sqlite_sequence';")
#define COUNT_DATA(TABLE_NAME)    (SELECT_TABLE"COUNT (*) FROM "+TABLE_NAME+";")
#define SELECT_TABLE_CALL_RECORD(TABLE_NAME) \
                                    (SELECT_TABLE" * FROM " + TABLE_NAME + \
                                    " ORDER BY " COLUMN_CALL_TIME " DESC ;")
#define SELECT_TABLE_CALL_RECORDNUM(TABLE_NAME) \
                                    (SELECT_TABLE " DISTINCT " COLUMN_CALL_NUMBER "  FROM "+TABLE_NAME+";")

#define SELECT_TABLE_CONTACT_BOOK(TABLE_NAME) \
                                    (SELECT_TABLE" * FROM " + TABLE_NAME+\
                                    " ORDER BY " COLUMN_CONTACT_FIRSTLETTERS +","+COLUMN_CONTACT_FIRSTLETTER " ASC ;")

#define SELECT_TABLE_CONTACT_BOOK_FILTER_FPY(TABLE_NAME,KEYWORD) \
    (SELECT_TABLE" * FROM " + TABLE_NAME + \
     " WHERE " COLUMN_CONTACT_FIRSTLETTER " LIKE '%" + KEYWORD + "%'"+\
    " ORDER BY " COLUMN_CONTACT_FIRSTLETTERS +","+COLUMN_CONTACT_FIRSTLETTER " ASC ;")

#define SELECT_TABLE_CONTACT_BOOK_FILTER_NUMBER(TABLE_NAME,KEYWORD) \
    (SELECT_TABLE " * FROM " + TABLE_NAME + \
    " WHERE " COLUMN_CONTACT_NUMBER " LIKE '%" + KEYWORD + "%' " + \
    " ORDER BY " COLUMN_CONTACT_FIRSTLETTERS +","+COLUMN_CONTACT_FIRSTLETTER " ASC ;")

#define SELECT_TABLE_CONTACT_BOOK_FILTER_PINYIN(TABLE_NAME,KEYWORD) \
    (SELECT_TABLE " * FROM " + TABLE_NAME + \
    " WHERE " COLUMN_CONTACT_NAME " LIKE '%" + KEYWORD + "%' " + \
   " ORDER BY " COLUMN_CONTACT_FIRSTLETTERS +","+COLUMN_CONTACT_FIRSTLETTER " ASC ;")
#define SELECT_TABLE_CONTACT_BOOK_FILTER_NAME_NUMBER(TABLE_NAME,KEYWORD) \
    (SELECT_TABLE " DISTINCT " COLUMN_CONTACT_NAME " FROM " + TABLE_NAME + \
    " WHERE " COLUMN_CONTACT_NUMBER "='" + KEYWORD +  \
    " ORDER BY " COLUMN_CONTACT_FIRSTLETTERS +","+COLUMN_CONTACT_FIRSTLETTER " ASC ;")

#define UPDATE_TABLE                "UPDATE "
#define UPDATE_TABLE_CALL_RECORD(TABLE_NAME,CALLNAME,CALLNUM) \
                                    (UPDATE_TABLE + TABLE_NAME + \
                                    " SET " COLUMN_CALLER_NAME" = '"+ CALLNAME + "' WHERE " COLUMN_CALL_NUMBER " = '" +CALLNUM +"';")
#define UPDATE_TABLE_CALL_RECORDALL(TABLE_NAME,CALLNAME) \
                                    (UPDATE_TABLE + TABLE_NAME + \
                                    " SET " COLUMN_CALLER_NAME" = '"+ CALLNAME +"';")

//#define UPDATE_TABLE_CALL_RECORD(TABLE_NAME,NAME,NUM) \
//                                    (DELETE_TABLE + TABLE_NAME + \
//                                    " WHERE " COLUMN_CONTACT_NAME" = '" + NAME +"' AND " COLUMN_CONTACT_NUMBER" = '" + NUM + "';")

#define DELETE_TABLE                "DELETE FROM "
#define DELETE_TABLE_CALL_RECORD_ALL(TABLE_NAME) \
                                    (DELETE_TABLE + TABLE_NAME +";")
#define DELETE_TABLE_CALL_RECORD_ID(TABLE_NAME,ID) \
                                    (DELETE_TABLE + TABLE_NAME + \
                                    " WHERE " COLUMN_CALL_TIME" = '" + ID + "';")
#define DELETE_TABLE_CALL_RECORD_EARLST(TABLE_NAME) \
                                    ( DELETE_TABLE + TABLE_NAME + " WHERE "+ COLUMN_CALL_RECORD_ID +\
                                    " IN ( SELECT "+ COLUMN_CALL_RECORD_ID + " FROM " + TABLE_NAME +" ORDER BY " +\
                                    COLUMN_CALL_TIME +" asc LIMIT 1);" )
#define DELETE_TABLE_CONTACT_BOOK_ALL(TABLE_NAME) \
                                    (DELETE_TABLE + TABLE_NAME +";")
#define DELETE_TABLE_CONTACT_BOOK_NAME(TABLE_NAME,ID) \
                                    (DELETE_TABLE + TABLE_NAME + \
                                    " WHERE " COLUMN_CONTACT_NAME" = '" + ID + "';")

#define DELETE_TABLE_CONTACT_BOOK_NUM(TABLE_NAME,ID) \
                                    (DELETE_TABLE + TABLE_NAME + \
                                    " WHERE " COLUMN_CONTACT_NUMBER" = '" + ID + "';")
#define DELETE_TABLE_CONTACT_BOOK_NAMENUM(TABLE_NAME,NAME,NUM) \
                                    (DELETE_TABLE + TABLE_NAME + \
                                    " WHERE " COLUMN_CONTACT_NAME" = '" + NAME +"' AND " COLUMN_CONTACT_NUMBER" = '" + NUM + "';")
//#define DELETE_TABLE_CALL_RECORD_ALL(TABLE_NAME) (DELETE_TABLE + TABLE_NAME)
//#define DELETE_TABLE_CONTACT_BOOK_ALL(TABLE_NAME) (DELETE_TABLE + TABLE_NAME)

#define INSERT_TABLE             "INSERT INTO "
#define INSERT_TABLE_CALL_RECORD(TABLE_NAME,VAL) \
                                (INSERT_TABLE + TABLE_NAME + \
                                 " ("  COLUMN_CALLER_NAME "," COLUMN_CALL_NUMBER "," COLUMN_CALL_TYPE "," COLUMN_CALL_TIME "," COLUMN_CALL_TALKTIME ")" \
                                 " VALUES (" + VAL+");")
#define INSERT_TABLE_CONTACT_BOOK(TABLE_NAME,VAL) \
                                (INSERT_TABLE + TABLE_NAME + \
                                " ("  COLUMN_CONTACT_NAME "," COLUMN_CONTACT_NUMBER "," COLUMN_CONTACT_TYPE "," COLUMN_CONTACT_FIRSTLETTERS "," COLUMN_CONTACT_FIRSTLETTER ")" \
                                " VALUES (" + VAL+");")

#endif // DATABASEDEFINE_H

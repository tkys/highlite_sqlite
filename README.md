# highlite_sqlite

## from source

`gcc -g -fPIC -shared snippet_ext.c -o snippet_ext.so`

## load extension file

```

$ sqlite3

sqlite> SELECT load_extension('./snippet_ext.so');
```

## how to use 

### sample

```
$ sqlite3

sqlite> SELECT load_extension('./snippet_ext.so');

sqlite> CREATE  TABLE tbl (text text);

sqlite> INSERT  INTO tbl VALUES ("hogefugahoge");



sqlite> SELECT snippet_ext(text, 'hoge', '<b>', '</b>') FROM tbl ;

<b>hoge</b>fuga<b>hoge</b>



sqlite> SELECT snippet_ext(text, 'g', '<b>', '</b>') FROM tbl ;

ho<b>g</b>efu<b>g</b>aho<b>g</b>e



```


```
sqlite> CREATE  TABLE books (id, title, text);
sqlite> INSERT  INTO books VALUES ( 0, "Search system", "Guide book for search devs");

sqlite> SELECT id, snippet_ext(title, 'Search', '<h1>', '</h1>'), snippet_ext(text , 'search', '<b>', '</b>') FROM books ;
0|<h1>Search</h1> system|Guide book for <b>search</b> devs

```

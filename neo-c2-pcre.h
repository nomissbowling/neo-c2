```
if echo $ARG | sed -e 's/-no-gc/NEOC_NO_GC/' | grep NEOC_NO_GC 1> /dev/null 2> /dev/null
then
    if [ "$COME_FLAG" = 1 ]
    then
        cat $PREFIX/include/neo-c2-pcre-declare.h | cpp -I . -U__GNUC__ -D_XOPEN_SOURCE
    elif [ -z "$COME_FLAG" ]
    then
        export COME_FLAG=1
        main_module=`neo-c2 function $SOURCE_NAME | fgrep 'main('`

        if [ -z "$main_module" ]
        then
            cat $PREFIX/include/neo-c2-pcre-declare.h | cpp -I . -U__GNUC__ -D_XOPEN_SOURCE
        else
            cat $PREFIX/include/neo-c2-pcre-main.h | cpp -I . -U__GNUC__ -D_XOPEN_SOURCE
        fi
    fi
else
    if [ "$COME_FLAG" = 1 ]
    then
        cat $PREFIX/include/neo-c2-pcre-declare-gc.h | cpp -I . -U__GNUC__ -D_XOPEN_SOURCE
    elif [ -z "$COME_FLAG" ]
    then
        export COME_FLAG=1
        main_module=`neo-c2 function $SOURCE_NAME | fgrep 'main('`

        if [ -z "$main_module" ]
        then
            cat $PREFIX/include/neo-c2-pcre-declare-gc.h | cpp -I . -U__GNUC__ -D_XOPEN_SOURCE
        else
            cat $PREFIX/include/neo-c2-pcre-main-gc.h | cpp -I . -U__GNUC__ -D_XOPEN_SOURCE
        fi
    fi
fi
```

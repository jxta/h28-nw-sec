
/*
 * client.c
 *    ソケットを使用して、サーバーに接続するクライアントプログラム。
 *    
 *    入力された文字列をサーバーに送り、サーバーが大文字に変換したデータを
 *    受け取る。
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_NAME    "/tmp/socket"  /* 通信に使うソケットファイル。サーバーと同じものを指定 */

main()
{
    struct sockaddr_un    addr;
    int    fd;
    int    len;
    char   buf[1024];
    int    ret;

    ret = 1024;

    /*
     *  ソケットを作る。このソケットはUNIXドメインで、ストリーム型ソケット。
     */
    if ((fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    /* 
     * addrの中身を0にしておかないと、bind()でエラーが起こることがある
     */
    bzero((char *)&addr, sizeof(addr));

    /*
     * ソケットの名前を入れておく
     */
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCK_NAME);

    /*
     *  サーバーとの接続を試みる。これが成功するためには、サーバーがすでに
     *  このアドレスをbind()して、listen()を発行していなければならない。
     */
    if (connect(fd, (struct sockaddr *)&addr,
             sizeof(addr.sun_family) + strlen(SOCK_NAME)) < 0){
        perror("connect");
        exit(1);
    }
    /*
     *  入力されたデータをソケットに書き込んでサーバーに送り、サーバーが変換して
     *  送ってきたデータを読み込む。
     */
    while (fgets(buf, 1024, stdin)) {
        write(fd, buf, 1024);
        ret = read(fd, buf, ret);
        /*write(1, buf, ret);*/
        printf("%s",buf);
    }
    close(fd);
    exit(0);
}

/*
 * server.c
 *   クライアントからの接続要求を受け付けるサーバープログラム。
 *   
 *   クライアントから送られてきた文字列を大文字に変換して送り返す。
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_NAME    "/tmp/socket"    /* 通信に使うソケットファイル */

main()
{
    int    i;
    int    fd1, fd2;
    struct sockaddr_un    saddr;
    struct sockaddr_un    caddr;
    int    len;
    int    ret;
    char   buf[1024];


    /*
     * ソケットを作る。このソケットはUNIXドメインで、ストリーム型ソケット。
     */
    if ((fd1 = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
	perror("socket");
	exit(1);
    }

    /* 
     * saddrの中身を0にしておかないと、bind()でエラーが起こることがある
     */
    bzero((char *)&saddr, sizeof(saddr));

    /*
     * ソケットの名前を入れておく
     */
    saddr.sun_family = AF_UNIX;
    strcpy(saddr.sun_path, SOCK_NAME);

    /*
     * ソケットにアドレスをバインドする。bind()が失敗しないよう、最初にunlink()を
     * 使用して、ソケットに対応するファイルを消去する。
     */
    unlink(SOCK_NAME);
    if (bind(fd1, (struct sockaddr *)&saddr, 
	     sizeof(saddr.sun_family) + strlen(SOCK_NAME)) < 0){
	perror("bind");
	exit(1);
    }
    
    /*
     * listenをソケットに対して発行する
     */
    if (listen(fd1, 1) < 0) {
	perror("listen");
	exit(1);
    }

    for(;;) {
	len = sizeof(caddr);
	/*
         * accept()により、クライアントからの接続要求を受け付ける。
         * 成功すると、クライアントと接続されたソケットのディスクリプタが
         * fd2に返される。このfd2を通して通信が可能となる。
	 * fd1は必要なくなるので、close()で閉じる。
         */
	if ((fd2 = accept(fd1, (struct sockaddr *)&caddr, &len)) < 0) {
	    perror("accept");
	    exit(1);
	}
	close(fd1);
       /* クライアントと接続されているソケットからデータを受け取る */
	while (ret = read(fd2, buf, 1024)) {
	    /* bufの中の小文字を大文字に変換する */
	    for (i=0; i< ret; i++) {
		if (isalpha(buf[i]))
		    buf[i] = toupper(buf[i]);
	    }
	    /* 変換したデータをクライアントに送り返す */
	    write(fd2, buf, 1024);
	}
	/* 通信が終わったらソケットを閉じる */
	close(fd2);
    }
}

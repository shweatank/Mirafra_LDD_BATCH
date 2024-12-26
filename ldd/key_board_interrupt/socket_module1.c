/* sudo code 
 *
 *
 * creating the socket/
 *
 * binding the socket
 *
 * listen
 *
 * send data
 *
 * recive data 
 *
 */

#include<linux/module.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/socket.h>
#include <linux/slab.h>
#include <linux/string.h>
#include "socket_module.h"

#define BUFFER_SIZE 1024
#define PORT 6543

static struct socket *server_sock;
static struct socket *client_sock;

/* Set up the socket server  */
static int setup_socket(void) {
    struct sockaddr_in server_addr;
    int ret;

    /* Create a socket */
    /*  AF_INET: IPv4 Internet protocols.
        AF_INET6: IPv6 Internet protocols.
        AF_UNIX or AF_LOCAL: Local inter-process communication (IPC). */

    /*  int type: Specifies the type of socket to be created. Common values:
     *  SOCK_STREAM: For stream-based communication (e.g., TCP).
     *  SOCK_DGRAM: For datagram-based communication (e.g., UDP).
     *  SOCK_RAW: For raw sockets that allow direct access to lower-level  */
    /*  IPPROTO_TCP: TCP protocol.
     *  IPPROTO_UDP: UDP protocol */
    ret = sock_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, &server_sock);
    if (ret < 0) {
        pr_err("Failed to create socket: %d\n", ret);
        return ret;
    }

    // Configure the server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    /* A special constant (0.0.0.0) that binds the socket to all available network interfaces.
     * Converts the IP address from host byte order to network byte order */
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    /*  Bind the socket   */
    /* sock: A pointer to the socket structure representing the socket to bind
     * addr: A pointer to a sockaddr structure that contains the IP address and port.  */
    ret = kernel_bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        pr_err("Failed to bind socket: %d\n", ret);
        sock_release(server_sock);
        return ret;
    }

    /* Start listening   */
    ret = kernel_listen(server_sock, 5);
    if (ret < 0) {
        pr_err("Failed to listen on socket: %d\n", ret);
        sock_release(server_sock);
        return ret;
    }

    pr_info("Socket server is ready on port %d\n", PORT);
    return 0;
}

/* Send data to the connected client and wait for acknowledgment  */
int socket_send_data(const char *data) {
    struct kvec *iov; /* A kernel-level I/O vector used for scatter-gather I/O operations.  */
    struct msghdr *msg; /* Contains message-related metadata */
    int ret;

    if (!client_sock) {
        pr_err("No client connected\n");
        return -ENOTCONN;
    }

    // Dynamically allocate memory for msg and iov to reduce stack usage
    iov = kmalloc(sizeof(struct kvec), GFP_KERNEL);
    /* It allows the kernel to block (wait) if memory isn’t immediately available  */
    msg = kmalloc(sizeof(struct msghdr), GFP_KERNEL);

    if (!iov || !msg) {
        pr_err("Failed to allocate memory for message structures\n");
        kfree(iov);
        kfree(msg);
        return -ENOMEM;
    }

    memset(msg, 0, sizeof(struct msghdr));
    iov->iov_base = (void *)data; /* pointing to the base address of the buffer */
    iov->iov_len = strlen(data);

    ret = kernel_sendmsg(client_sock, msg, iov, 1, strlen(data));
    if (ret < 0) {
        pr_err("Failed to send data: %d\n", ret);
    }

    kfree(iov);
    kfree(msg);

    return ret;
}

EXPORT_SYMBOL(socket_send_data); // Export the function for external modules

static int __init socket_module_init(void) {
    int ret;

    pr_info("Initializing socket module\n");

    /* Set up the server socket   */
    ret = setup_socket();
    if (ret < 0) {
        pr_err("Socket setup failed\n");
        return ret;
    }

    /* Accept a client connection  */
    ret = kernel_accept(server_sock, &client_sock, 0);
    if (ret < 0) {
        pr_err("Failed to accept client: %d\n", ret);
        sock_release(server_sock);
        return ret;
    }

    pr_info("Client connected successfully\n");
    return 0;
}

static void __exit socket_module_exit(void) {
    if (client_sock) {
        sock_release(client_sock);
    }
    if (server_sock) {
        sock_release(server_sock);
    }
    pr_info("Socket module exited\n");
}

module_init(socket_module_init);
module_exit(socket_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yaswanth Reddy");
MODULE_DESCRIPTION("Socket Module with ACK Support");


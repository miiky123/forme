import socket
import sys


def main():
    # Expect exactly two arguments: server IP and server port
    if len(sys.argv) != 3:
        print("400 Bad Request")
        return

    # Server address provided by the user
    host = sys.argv[1]
    port = int(sys.argv[2])

    # Create a TCP socket (IPv4, stream-oriented)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # Establish a single TCP connection with the server
    s.connect((host, port))

    try:
        while True:
            try:
                # Read one command from the user (stdin)
                # The client does not validate or interpret commands
                line = input()
            except EOFError:
                # Stop if input stream ends (Ctrl+D)
                break

            # The protocol is line-based: send the command with a newline
            s.sendall((line + "\n").encode("utf-8"))

            # Wait for the server's response BEFORE reading the next command.
            # We keep reading until we see that the reply ends with '\n',
            # which matches the protocol definition.
            reply_chunks = []
            while True:
                data = s.recv(4096)
                if not data:
                    # If the server closed the connection, stop the client
                    return

                text = data.decode("utf-8")
                reply_chunks.append(text)

                # Stop when the *whole* reply for this command ends with '\n'
                if text.endswith("\n"):
                    break

            reply = "".join(reply_chunks)
            sys.stdout.write(reply)
            sys.stdout.flush()

    except KeyboardInterrupt:
        # Allow the user to exit with Ctrl+C gracefully
        pass
    finally:
        # Always close the TCP connection on exit
        s.close()


if __name__ == "__main__":
    main()

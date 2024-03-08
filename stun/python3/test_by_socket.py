import socket

def get_public_ip():
    # Google's STUN server address and port
    stun_server = ("stun.l.google.com", 19302)

    # Create a UDP socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Send a STUN binding request
    s.sendto(b"\x00\x01\x00\x08\x00\x10\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", stun_server)

    # Receive the response
    response, _ = s.recvfrom(1024)

    # Extract the public IP address from the response
    public_ip = '.'.join(str(byte) for byte in response[20:24])

    # Close the socket
    s.close()

    return public_ip

if __name__ == "__main__":
    public_ip = get_public_ip()
    print("Public IP Address:", public_ip)

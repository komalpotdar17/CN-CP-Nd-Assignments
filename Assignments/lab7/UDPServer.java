import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class UDPServer {
    public static void main(String[] args) {
        try {
            // Create a socket to listen on port 9876
            DatagramSocket serverSocket = new DatagramSocket(9876);  
            byte[] receiveData = new byte[1024];
            byte[] sendData;

            System.out.println("Server is ready and waiting for messages...");

            while (true) {
                // Receive data from the client
                DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                serverSocket.receive(receivePacket);

                // Extract message from received packet
                String message = new String(receivePacket.getData(), 0, receivePacket.getLength());
                System.out.println("Received: " + message);

                // Prepare the response message
                InetAddress clientAddress = receivePacket.getAddress();
                int clientPort = receivePacket.getPort();
                String response = "Echo: " + message;
                sendData = response.getBytes();

                // Send response to the client
                DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, clientAddress, clientPort);
                serverSocket.send(sendPacket);
                System.out.println("Response sent: " + response);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}

import asyncio
from aioquic.asyncio import serve
from aioquic.quic.configuration import QuicConfiguration
from aioquic.asyncio.protocol import QuicConnectionProtocol
from aioquic.quic.events import HandshakeCompleted, StreamDataReceived

class SimpleQuicProtocol(QuicConnectionProtocol):
    def __init__(self, connection, configuration):
        super().__init__(connection, configuration)
        print("Server initialized and waiting for connections...")

    def quic_event_received(self, event):
        if isinstance(event, HandshakeCompleted):
            print(f"Handshake completed with {self._quic.peer_address}")
        elif isinstance(event, StreamDataReceived):
            stream_id = event.stream_id
            data = event.data
            print(f"Received data: {data.decode()} on stream {stream_id}")
            # Send a response back to the client
            self._quic.send_stream_data(stream_id, b"Response from server.", end_stream=True)

async def run_quic_server():
    try:
        configuration = QuicConfiguration(is_client=False)
        configuration.load_cert_chain(certfile="cert.pem", keyfile="key.pem")

        print("Configuration loaded. Starting server...")
        await serve("0.0.0.0", 4433, configuration=configuration, create_protocol=SimpleQuicProtocol)
        
        print("Server started successfully. Waiting for connections...")
        
        # Keep the server running indefinitely
        stop_event = asyncio.Event()
        await stop_event.wait()
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    asyncio.run(run_quic_server())

from aioquic.asyncio import connect
from aioquic.quic.configuration import QuicConfiguration
import asyncio

async def run_quic_client():
    configuration = QuicConfiguration(is_client=True)
    configuration.verify_mode = False  # Disable certificate verification for testing

    print("Connecting to the server on localhost:4433...")
    try:
        async with connect("localhost", 4433, configuration=configuration) as protocol:
            print("Connected to server.")
            stream_id = protocol._quic.get_next_available_stream_id()

            print("Sending USER command...")
            protocol._quic.send_stream_data(stream_id, b"USER anonymous", end_stream=False)

            print("Sending PASS command...")
            protocol._quic.send_stream_data(stream_id, b"PASS anonymous", end_stream=False)

            print("Sending LIST command...")
            protocol._quic.send_stream_data(stream_id, b"LIST", end_stream=True)

            # Wait to receive the response
            print("Waiting for response...")
            response = protocol._quic.receive_stream_data(stream_id)
            print(f"Received response: {response.decode()}")
            
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    asyncio.run(run_quic_client())

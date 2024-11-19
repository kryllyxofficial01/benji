from http.server import BaseHTTPRequestHandler, HTTPServer
import json

class Handler(BaseHTTPRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        data = json.loads(post_data.decode('utf-8'))

        print("Received POST data:", data)

        self.send_response(200)
        self.send_header("Connection", "keep-alive")
        self.end_headers()

if __name__ == '__main__':
    server_address = ('127.0.0.1', 8080)

    httpd = HTTPServer(server_address, Handler)

    print('Starting server...')

    httpd.serve_forever()
const http = require('http');
const ntpClient = require('ntp-client');

const server = http.createServer((req, res) => {
    res.writeHead(200, {'Content-Type': 'text/plain'});

    // Obter o tempo do servidor NTP
    ntpClient.getNetworkTime('time.nist.gov', 123, function(err, date) {
        if (err) {
            res.end('Erro ao obter hora do servidor NTP');
            return;
        }
        res.end(date.toString());
    });
});

const PORT = 8081;
server.listen(PORT, () => {
    console.log(`Servidor Node.js rodando em http://localhost:${PORT}/`);
});

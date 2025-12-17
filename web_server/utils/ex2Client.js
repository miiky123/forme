const net = require('net');
const {
  BadRequestError,
  NotFoundError,
  UpstreamError
} = require('./httpErrors');

const host = process.env.EX2_HOST || '127.0.0.1';
const port = parseInt(process.env.EX2_PORT || '9090', 10);
const timeoutMs = parseInt(process.env.EX2_TIMEOUT || '4000', 10);

function sendCommand(commandLine) {
  return new Promise((resolve, reject) => {
    let resolved = false;
    let buffer = '';
    let statusLine = null;
    let stage = 'status'; // status | separator | body | done
    const socket = net.createConnection({ host, port });

    const cleanup = (err, result) => {
      if (resolved) return;
      resolved = true;
      socket.destroy();
      if (err) {
        reject(err);
      } else {
        resolve(result);
      }
    };

    const handleStatusLine = (line) => {
      const normalized = line.replace(/\r$/, '');
      if (normalized === '200 Ok') {
        stage = 'separator';
        return;
      }
      if (normalized === '201 Created') {
        cleanup(null, { status: 201 });
        return;
      }
      if (normalized === '204 No Content') {
        cleanup(null, { status: 204 });
        return;
      }
      if (normalized === '400 Bad Request') {
        cleanup(new BadRequestError('Bad Request'));
        return;
      }
      if (normalized === '404 Not Found') {
        cleanup(new NotFoundError('Not Found'));
        return;
      }
      cleanup(new UpstreamError('Upstream protocol error'));
    };

    const processBuffer = () => {
      if (resolved) return;

      if (stage === 'status') {
        const nlIndex = buffer.indexOf('\n');
        if (nlIndex === -1) return;
        statusLine = buffer.slice(0, nlIndex);
        buffer = buffer.slice(nlIndex + 1);
        handleStatusLine(statusLine);
        if (resolved || stage === 'done' || stage === 'status') return;
      }

      if (stage === 'separator') {
        const sepIndex = buffer.indexOf('\n\n');
        if (sepIndex === -1) return;
        buffer = buffer.slice(sepIndex + 2);
        stage = 'body';
      }

      if (stage === 'body') {
        const nlIndex = buffer.indexOf('\n');
        if (nlIndex === -1) return;
        const bodyLine = buffer.slice(0, nlIndex);
        stage = 'done';
        cleanup(null, { status: 200, body: bodyLine });
      }
    };

    socket.setTimeout(timeoutMs, () => {
      cleanup(new UpstreamError('Upstream timeout'));
    });

    socket.on('error', (err) => {
      cleanup(new UpstreamError(err.message));
    });

    socket.on('data', (chunk) => {
      buffer += chunk.toString('utf8');
      processBuffer();
    });

    const command = commandLine.endsWith('\n') ? commandLine : `${commandLine}\n`;
    socket.write(command, (err) => {
      if (err) {
        cleanup(new UpstreamError(err.message));
        return;
      }
      socket.end();
    });
  });
}

async function createFile(key, content = '') {
  const prefix = content === '' ? `post ${key}` : `post ${key} ${content}`;
  await sendCommand(`${prefix}\n`);
}

async function getFile(key) {
  const result = await sendCommand(`get ${key}\n`);
  return result.body || '';
}

async function deleteFile(key) {
  await sendCommand(`delete ${key}\n`);
}

async function search(query) {
  if (!query || query.trim() === '') {
    return [];
  }
  const result = await sendCommand(`search ${query}\n`);
  if (!result.body) {
    return [];
  }
  return result.body
    .trim()
    .split(/\s+/)
    .filter(Boolean);
}

module.exports = {
  createFile,
  getFile,
  deleteFile,
  search
};

const filesRepo = require('../repositories/files.repository');
const ex2Client = require('../utils/ex2Client');
const { BadRequestError, NotFoundError } = require('../utils/httpErrors');

async function listRoot(userId) {
  return filesRepo.listByUser(userId);
}

async function create(userId, payload) {
  if (!payload || !payload.name) {
    throw new BadRequestError('Name is required');
  }

  const record = filesRepo.create(userId, {
    name: payload.name,
    type: payload.type || 'file'
  });

  const backendKey = `${userId}__${record.id}`;
  filesRepo.updateBackendKey(userId, record.id, backendKey);

  try {
    if (record.type === 'file') {
      await ex2Client.createFile(backendKey, payload.content || '');
    }
    return filesRepo.findById(userId, record.id);
  } catch (err) {
    filesRepo.deleteById(userId, record.id);
    throw err;
  }
}

async function getById(userId, id) {
  const record = filesRepo.findById(userId, id);
  if (!record) {
    throw new NotFoundError('File not found');
  }

  if (record.type !== 'file' || !record.backendKey) {
    return record;
  }

  const content = await ex2Client.getFile(record.backendKey);
  return { ...record, content };
}

async function remove(userId, id) {
  const record = filesRepo.findById(userId, id);
  if (!record) {
    throw new NotFoundError('File not found');
  }

  if (record.type === 'file' && record.backendKey) {
    await ex2Client.deleteFile(record.backendKey);
  }

  filesRepo.deleteById(userId, id);
}

module.exports = {
  listRoot,
  create,
  getById,
  remove
};

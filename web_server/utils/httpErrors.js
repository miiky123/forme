class HttpError extends Error {
  constructor(status, message) {
    super(message);
    this.status = status;
  }
}

function BadRequestError(message = 'Bad Request') {
  return new HttpError(400, message);
}

function NotFoundError(message = 'Not Found') {
  return new HttpError(404, message);
}

function UnauthorizedError(message = 'Unauthorized') {
  return new HttpError(401, message);
}

function ForbiddenError(message = 'Forbidden') {
  return new HttpError(403, message);
}

function UpstreamError(message = 'Bad Gateway') {
  return new HttpError(502, message);
}

module.exports = {
  HttpError,
  BadRequestError,
  NotFoundError,
  UnauthorizedError,
  ForbiddenError,
  UpstreamError
};

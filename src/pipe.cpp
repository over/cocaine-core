/*
    Copyright (c) 2011-2012 Andrey Sibiryov <me@kobology.ru>
    Copyright (c) 2011-2012 Other contributors as noted in the AUTHORS file.

    This file is part of Cocaine.

    Cocaine is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Cocaine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "cocaine/asio/pipe.hpp"

using namespace cocaine::io;

pipe_t::pipe_t(pipe_t&& other):
    m_fd(-1)
{
    *this = std::move(other);
}

pipe_t&
pipe_t::operator=(pipe_t&& other) {
    std::swap(m_fd, other.m_fd);
    return *this;
}

pipe_t::~pipe_t() {
    if(m_fd != -1 && ::close(m_fd) != 0) {
        // Log.
    }
}

ssize_t
pipe_t::write(const char * buffer,
              size_t size)
{
    ssize_t length = ::write(m_fd, buffer, size);

    if(length == -1) {
        switch(errno) {
            case EAGAIN:
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK:
#endif
            case EINTR:
                return length;

            default:
                throw io_error_t("unable to write to a pipe");
        }
    }

    return length;
}

ssize_t
pipe_t::read(char * buffer,
             size_t size)
{
    ssize_t length = ::read(m_fd, buffer, size);

    if(length == -1) {
        switch(errno) {
            case EAGAIN:
#if defined(EWOULDBLOCK) && EWOULDBLOCK != EAGAIN
            case EWOULDBLOCK:
#endif
            case EINTR:
                return length;

            default:
                throw io_error_t("unable to read from a pipe");
        }
    }

    return length;
}

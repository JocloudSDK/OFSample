# -*- coding: utf-8 -*-

u'''
    http file upload/download的实现
    upload的代码来自http://pymotw.com/2/urllib2/index.html#uploading-files
    构建脚本目前不依赖requests，所以仍旧采用urllib2

    upload/download成功将直接返回，出错则会抛出异常。
    需要区分URLError和HTTPError，分别处理。
    HTTPError：http response能正确接收，但是返回码不是200。
    URLError：http通信出错，比如TCP连接无法建立。
'''

import itertools
import logging
import mimetools
import mimetypes
import urllib2

class _MultiPartForm(object):
    '''Accumulate the data to be used when posting a form.'''

    def __init__(self):
        self.form_fields = []
        self.files = []
        self.boundary = mimetools.choose_boundary()

    def get_content_type(self):
        return 'multipart/form-data; boundary=%s' % self.boundary

    def add_field(self, name, value):
        '''Add a simple field to the form data.'''
        self.form_fields.append((name, value))

    def add_file(self, fieldname, filename, fileHandle, mimetype=None):
        '''Add a file to be uploaded.'''
        body = fileHandle.read()
        if mimetype is None:
            mimetype = mimetypes.guess_type(filename)[0] or 'application/octet-stream'
        self.files.append((fieldname, filename, mimetype, body))

    def __str__(self):
        '''Return a string representing the form data, including attached files.'''
        # Build a list of lists, each containing 'lines' of the
        # request.  Each part is separated by a boundary string.
        # Once the list is built, return a string where each
        # line is separated by '\r\n'.
        parts = []
        part_boundary = '--' + self.boundary

        # Add the form fields
        parts.extend(
            [
                part_boundary,
                'Content-Disposition: form-data; name="%s"' % name,
                '',
                value,
            ]
            for name, value in self.form_fields
        )

        # Add the files to upload
        parts.extend(
            [
                part_boundary,
                'Content-Disposition: file; name="%s"; filename="%s"' % \
                    (field_name, filename),
                'Content-Type: %s' % content_type,
                '',
                body,
            ]
            for field_name, filename, content_type, body in self.files
        )

        # Flatten the list and add closing boundary marker,
        # then return CR+LF separated data
        flattened = list(itertools.chain(*parts))
        flattened.append('--' + self.boundary + '--')
        flattened.append('')
        return '\r\n'.join(flattened)


def upload(url, field_name, file_name, file_path):
    with open(file_path, 'rb') as f:
        form = _MultiPartForm()
        form.add_file(field_name, file_name, fileHandle=f)

        # Build the request
        request = urllib2.Request(url)
        body = str(form)
        request.add_header('Content-type', form.get_content_type())
        request.add_header('Content-length', len(body))
        request.add_data(body)

        response = urllib2.urlopen(request)
        logging.debug(response.read())


def download(url, dest_file_path):
    response = urllib2.urlopen(url)
    if response is not None:
        with open(dest_file_path, 'wb') as w:
            w.write(response.read())

/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2015, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
copyright notice, this list of conditions and the
following disclaimer.

* Redistributions in binary form must reproduce the above
copyright notice, this list of conditions and the
following disclaimer in the documentation and/or other
materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
contributors may be used to endorse or promote products
derived from this software without specific prior
written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/
#include "glTFImporter.h"
#include "StreamReader.h"
#include "DefaultIOSystem.h"

#include <boost/scoped_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/ai_assert.h>
#include <assimp/DefaultLogger.hpp>

#include "../contrib/picojson/picojson.h"

namespace Assimp {

    static const aiImporterDesc desc = {
        "glTF Importer",
        "",
        "",
        "",
        aiImporterFlags_SupportTextFlavour | aiImporterFlags_SupportCompressedFlavour,
        0,
        0,
        0,
        0,
        "gltf"
    };

glTFImporter::glTFImporter() 
: BaseImporter()
, m_scene( NULL ) 
, m_buffer() {

}

glTFImporter::~glTFImporter() {

}

bool glTFImporter::CanRead( const std::string& pFile, IOSystem* pIOHandler, bool checkSig ) const {
    return false;
}

const aiImporterDesc* glTFImporter::GetInfo() const {
    return &desc;
}

void glTFImporter::InternReadFile( const std::string& pFile, aiScene* pScene, IOSystem* pIOHandler ) {
    m_scene = pScene;
    boost::shared_ptr<IOStream> stream( pIOHandler->Open( pFile, "rb" ) );
    if (!stream.get()) {
        throw DeadlyImportError( "Failed to open file " + pFile + "." );
    }

    // Get the file-size and validate it, throwing an exception when fails
    size_t fileSize = stream->FileSize();

    // Allocate buffer and read file into it
    TextFileToBuffer( stream.get(), m_buffer );
}

}
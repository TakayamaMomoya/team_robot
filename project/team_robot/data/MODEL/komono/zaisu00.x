xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 56;
 -9.70000;0.50015;-10.00000;,
 9.70000;0.50015;-10.00000;,
 9.70000;0.37832;-10.00000;,
 -9.70000;0.37832;-10.00000;,
 9.22807;0.80015;-9.22807;,
 -9.22807;0.80015;-9.22807;,
 10.00000;0.37832;-9.70000;,
 10.00000;0.50015;-9.70000;,
 -9.70000;0.07832;-9.70000;,
 9.70000;0.07832;-9.70000;,
 -10.00000;0.50015;-9.70000;,
 -10.00000;0.37832;-9.70000;,
 10.00000;0.50015;9.70000;,
 10.00000;0.37832;9.70000;,
 9.22807;0.80015;9.22807;,
 9.70000;0.37832;10.00000;,
 9.70000;0.50015;10.00000;,
 9.70000;0.07832;9.70000;,
 -9.70000;0.50015;10.00000;,
 -9.70000;0.37832;10.00000;,
 -9.22807;0.80015;9.22807;,
 -10.00000;0.37832;9.70000;,
 -10.00000;0.50015;9.70000;,
 -9.70000;0.07832;9.70000;,
 7.76275;16.50154;-8.96827;,
 8.78618;16.50154;-8.96827;,
 8.78618;0.64535;-9.06123;,
 7.76275;0.64535;-9.06123;,
 8.78618;16.50154;-7.67851;,
 8.78618;0.64535;-7.58555;,
 7.76275;16.50154;-7.67851;,
 7.76275;0.64535;-7.58555;,
 -8.81352;16.46946;-8.96827;,
 -7.79010;16.46946;-8.96827;,
 -7.79010;0.61326;-9.06123;,
 -8.81352;0.61326;-9.06123;,
 -7.79010;16.46946;-7.67851;,
 -8.81352;16.46946;-7.67851;,
 -8.81352;0.61326;-7.58555;,
 -7.79010;0.61326;-7.58555;,
 -9.90474;16.47160;-8.96827;,
 -9.90474;17.24453;-8.96827;,
 10.09525;17.24453;-9.06123;,
 10.09526;16.47160;-9.06123;,
 -9.90474;17.24453;-7.67851;,
 -9.90474;16.47160;-7.67851;,
 10.09526;16.47160;-7.58555;,
 10.09525;17.24453;-7.58555;,
 -3.11273;16.59288;-8.43537;,
 3.11273;16.59288;-8.43537;,
 3.11273;0.73668;-8.43537;,
 -3.11273;0.73668;-8.43537;,
 3.11273;16.59288;-7.98818;,
 3.11273;0.73668;-7.98818;,
 -3.11273;16.59288;-7.98818;,
 -3.11273;0.73668;-7.98818;;
 
 50;
 4;0,1,2,3;,
 4;4,1,0,5;,
 4;6,2,1,7;,
 4;8,3,2,9;,
 4;10,0,3,11;,
 3;10,5,0;,
 3;4,7,1;,
 3;6,9,2;,
 3;8,11,3;,
 4;7,12,13,6;,
 4;14,12,7,4;,
 4;15,13,12,16;,
 4;9,6,13,17;,
 3;14,16,12;,
 3;15,17,13;,
 4;16,18,19,15;,
 4;20,18,16,14;,
 4;21,19,18,22;,
 4;17,15,19,23;,
 3;20,22,18;,
 3;21,23,19;,
 4;22,10,11,21;,
 4;5,10,22,20;,
 4;23,21,11,8;,
 4;20,14,4,5;,
 4;8,9,17,23;,
 4;24,25,26,27;,
 4;25,28,29,26;,
 4;28,30,31,29;,
 4;30,24,27,31;,
 4;30,28,25,24;,
 4;27,26,29,31;,
 4;32,33,34,35;,
 4;36,37,38,39;,
 4;37,32,35,38;,
 4;37,36,33,32;,
 4;35,34,39,38;,
 4;33,36,39,34;,
 4;40,41,42,43;,
 4;44,45,46,47;,
 4;45,40,43,46;,
 4;45,44,41,40;,
 4;43,42,47,46;,
 4;41,44,47,42;,
 4;48,49,50,51;,
 4;49,52,53,50;,
 4;52,54,55,53;,
 4;54,48,51,55;,
 4;54,52,49,48;,
 4;51,50,53,55;;
 
 MeshMaterialList {
  3;
  50;
  2,
  2,
  2,
  2,
  1,
  1,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  2,
  2,
  2,
  2,
  0,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\wood01.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\wood-texture_00019.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\wood-texture_00019.jpg";
   }
  }
 }
}

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
 44;
 0.00000;30.75115;-1.19470;,
 0.84478;30.75115;-0.84478;,
 0.84478;0.31034;-0.84478;,
 0.00000;0.31034;-1.19470;,
 1.19470;30.75115;0.00000;,
 1.19470;0.31034;0.00000;,
 0.84478;30.75115;0.84478;,
 0.84478;0.31034;0.84478;,
 0.00000;30.75115;1.19470;,
 0.00000;0.31034;1.19470;,
 -0.84478;30.75115;0.84478;,
 -0.84478;0.31034;0.84478;,
 -1.19470;30.75115;0.00000;,
 -1.19470;0.31034;0.00000;,
 -0.84478;30.75115;-0.84478;,
 -0.84478;0.31034;-0.84478;,
 0.00000;30.75115;0.00000;,
 0.00000;0.31034;0.00000;,
 -1.00333;31.52608;-0.27985;,
 1.00333;31.52608;-0.27985;,
 1.00333;30.52785;-0.27985;,
 -1.00333;30.52785;-0.27985;,
 1.00333;31.52608;0.27985;,
 1.00333;30.52785;0.27985;,
 -1.00333;31.52608;0.27985;,
 -1.00333;30.52785;0.27985;,
 0.00000;37.11599;0.25038;,
 2.04238;36.27000;0.25038;,
 2.04238;36.27000;-0.27139;,
 0.00000;37.11599;-0.27139;,
 2.88835;34.22763;0.25038;,
 2.88835;34.22763;-0.27139;,
 2.04238;32.18525;0.25038;,
 2.04238;32.18525;-0.27139;,
 0.00000;31.33928;0.25038;,
 0.00000;31.33928;-0.27139;,
 -2.04238;32.18525;0.25038;,
 -2.04238;32.18525;-0.27139;,
 -2.88835;34.22763;0.25038;,
 -2.88835;34.22763;-0.27139;,
 -2.04238;36.27000;0.25038;,
 -2.04238;36.27000;-0.27139;,
 0.00000;34.22763;0.25038;,
 0.00000;34.22763;-0.27139;;
 
 54;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;16,1,0;,
 3;16,4,1;,
 3;16,6,4;,
 3;16,8,6;,
 3;16,10,8;,
 3;16,12,10;,
 3;16,14,12;,
 3;16,0,14;,
 3;17,3,2;,
 3;17,2,5;,
 3;17,5,7;,
 3;17,7,9;,
 3;17,9,11;,
 3;17,11,13;,
 3;17,13,15;,
 3;17,15,3;,
 4;18,19,20,21;,
 4;19,22,23,20;,
 4;22,24,25,23;,
 4;24,18,21,25;,
 4;24,22,19,18;,
 4;21,20,23,25;,
 4;26,27,28,29;,
 4;27,30,31,28;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,26,29,41;,
 3;42,27,26;,
 3;42,30,27;,
 3;42,32,30;,
 3;42,34,32;,
 3;42,36,34;,
 3;42,38,36;,
 3;42,40,38;,
 3;42,26,40;,
 3;43,29,28;,
 3;43,28,31;,
 3;43,31,33;,
 3;43,33,35;,
 3;43,35,37;,
 3;43,37,39;,
 3;43,39,41;,
 3;43,41,29;;
 
 MeshMaterialList {
  3;
  54;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.200000;0.200000;0.200000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\derinater.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
}

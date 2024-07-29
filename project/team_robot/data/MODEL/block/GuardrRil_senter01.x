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
 27.58066;19.63102;-1.39674;,
 28.70322;19.63102;-0.93638;,
 28.70322;-0.36898;-0.93638;,
 27.58066;-0.36898;-1.39674;,
 29.16819;19.63102;0.17503;,
 29.16819;-0.36898;0.17503;,
 28.70322;19.63102;1.28645;,
 28.70322;-0.36898;1.28645;,
 27.58066;19.63102;1.74681;,
 27.58066;-0.36898;1.74681;,
 26.45810;19.63102;1.28645;,
 26.45810;-0.36898;1.28645;,
 25.99312;19.63102;0.17503;,
 25.99312;-0.36898;0.17503;,
 26.45810;19.63102;-0.93638;,
 26.45810;-0.36898;-0.93638;,
 27.58066;19.63102;0.17503;,
 27.58066;-0.36898;0.17503;,
 -27.89996;19.63102;-1.36605;,
 -26.77740;19.63102;-0.90569;,
 -26.77740;-0.36898;-0.90569;,
 -27.89996;-0.36898;-1.36605;,
 -26.31243;19.63102;0.20573;,
 -26.31243;-0.36898;0.20573;,
 -26.77740;19.63102;1.31714;,
 -26.77740;-0.36898;1.31714;,
 -27.89996;19.63102;1.77750;,
 -27.89996;-0.36898;1.77750;,
 -29.02252;19.63102;1.31714;,
 -29.02252;-0.36898;1.31714;,
 -29.48749;19.63102;0.20573;,
 -29.48749;-0.36898;0.20573;,
 -29.02252;19.63102;-0.90569;,
 -29.02252;-0.36898;-0.90569;,
 -27.89996;19.63102;0.20573;,
 -27.89996;-0.36898;0.20573;,
 -34.02747;19.23596;0.41156;,
 33.69690;19.23596;0.41156;,
 33.69690;10.19484;0.41156;,
 -34.02747;10.19484;0.41156;,
 33.69690;19.23596;0.49158;,
 33.69690;10.19484;0.49158;,
 -34.02747;19.23596;0.49158;,
 -34.02747;10.19484;0.49158;;
 
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
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 4;32,18,21,33;,
 3;34,19,18;,
 3;34,22,19;,
 3;34,24,22;,
 3;34,26,24;,
 3;34,28,26;,
 3;34,30,28;,
 3;34,32,30;,
 3;34,18,32;,
 3;35,21,20;,
 3;35,20,23;,
 3;35,23,25;,
 3;35,25,27;,
 3;35,27,29;,
 3;35,29,31;,
 3;35,31,33;,
 3;35,33,21;,
 4;36,37,38,39;,
 4;37,40,41,38;,
 4;40,42,43,41;,
 4;42,36,39,43;,
 4;42,40,37,36;,
 4;39,38,41,43;;
 
 MeshMaterialList {
  3;
  54;
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
  0,
  0,
  0,
  1,
  2,
  1,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Pictures\\team\\metal_00003.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Pictures\\team\\GardRail01.png";
   }
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
}

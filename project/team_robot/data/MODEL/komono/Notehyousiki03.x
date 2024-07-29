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
 42;
 0.00000;75.44036;-1.59547;,
 1.03646;75.44036;-1.12817;,
 1.03646;0.26141;-1.12817;,
 0.00000;0.26141;-1.59547;,
 1.46577;75.44036;0.00000;,
 1.46577;0.26141;0.00000;,
 1.03646;75.44036;1.12817;,
 1.03646;0.26141;1.12817;,
 0.00000;75.44036;1.59547;,
 0.00000;0.26141;1.59547;,
 -1.03646;75.44036;1.12817;,
 -1.03646;0.26141;1.12817;,
 -1.46577;75.44036;0.00000;,
 -1.46577;0.26141;0.00000;,
 -1.03646;75.44036;-1.12817;,
 -1.03646;0.26141;-1.12817;,
 0.00000;75.44036;0.00000;,
 0.00000;0.26141;0.00000;,
 -12.66581;64.82159;-1.61108;,
 -0.83974;76.65969;-1.61108;,
 -0.61602;76.20089;-1.61108;,
 -12.20702;64.60339;-1.61108;,
 -0.35211;75.65969;-1.61108;,
 -11.66582;64.34598;-1.61108;,
 -0.08557;52.49108;-1.61108;,
 11.50279;64.08599;-1.61108;,
 11.96160;63.86559;-1.61108;,
 0.14040;52.03228;-1.61108;,
 -0.35211;53.03228;-1.61108;,
 10.96159;64.34598;-1.61108;,
 -0.08821;76.20089;-1.61108;,
 11.50279;64.60598;-1.61108;,
 0.13552;76.65969;-1.61108;,
 11.96160;64.82637;-1.61108;,
 -12.20702;64.08858;-1.61108;,
 -0.61866;52.49108;-1.61108;,
 -0.84463;52.03228;-1.61108;,
 -12.66582;63.87037;-1.61108;,
 -0.29745;76.85417;-1.61108;,
 -12.86029;64.29266;-1.61108;,
 12.15607;64.39982;-1.61108;,
 -0.40731;51.83779;-1.61108;;
 
 73;
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
 4;21,20,22,23;,
 4;24,25,26,27;,
 4;28,29,25,24;,
 4;29,22,30,31;,
 4;31,30,32,33;,
 4;34,35,36,37;,
 3;19,38,20;,
 3;38,32,30;,
 3;30,20,38;,
 3;20,30,22;,
 3;21,39,18;,
 3;34,37,39;,
 3;39,21,34;,
 3;23,34,21;,
 3;25,40,26;,
 3;31,33,40;,
 3;40,25,31;,
 3;29,31,25;,
 3;27,41,24;,
 3;41,36,35;,
 3;35,24,41;,
 3;24,35,28;,
 4;28,29,22,23;,
 4;21,20,19,18;,
 4;23,22,20,21;,
 4;27,26,25,24;,
 4;24,25,29,28;,
 4;31,30,22,29;,
 4;33,32,30,31;,
 4;34,35,28,23;,
 4;37,36,35,34;,
 3;20,38,19;,
 3;30,32,38;,
 3;38,20,30;,
 3;22,30,20;,
 3;18,39,21;,
 3;39,37,34;,
 3;34,21,39;,
 3;21,34,23;,
 3;26,40,25;,
 3;40,33,31;,
 3;31,25,40;,
 3;25,31,29;,
 3;24,41,27;,
 3;35,36,41;,
 3;41,24,35;,
 3;28,35,24;,
 4;22,29,35,34;;
 
 MeshMaterialList {
  3;
  73;
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
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.150000;0.150000;0.150000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\metal_00003.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\sika.png";
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

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
 46;
 0.00000;145.95452;-1.46577;,
 0.96175;145.95452;-1.03646;,
 0.96175;0.09320;-1.03646;,
 0.00000;0.09320;-1.46577;,
 1.36011;145.95452;0.00000;,
 1.36011;0.09320;0.00000;,
 0.96175;145.95452;1.03646;,
 0.96175;0.09320;1.03646;,
 -0.00000;145.95452;1.46577;,
 -0.00000;0.09320;1.46577;,
 -0.96175;145.95452;1.03646;,
 -0.96175;0.09320;1.03646;,
 -1.36011;145.95452;-0.00000;,
 -1.36011;0.09320;-0.00000;,
 -0.96175;145.95452;-1.03646;,
 -0.96175;0.09320;-1.03646;,
 0.00000;145.95452;0.00000;,
 0.00000;0.09320;0.00000;,
 -3.17431;145.46732;1.02461;,
 3.40638;145.46732;1.02461;,
 3.40638;145.23363;1.02461;,
 -3.17431;145.23363;1.02461;,
 3.40638;145.86732;1.26753;,
 -3.17431;145.86732;1.26752;,
 3.67746;145.23363;1.26752;,
 3.67746;145.46732;1.26752;,
 -3.17431;144.83363;1.26752;,
 3.40638;144.83363;1.26753;,
 -3.44539;145.46732;1.26752;,
 -3.44539;145.23363;1.26752;,
 5.37110;145.46732;12.92742;,
 5.37110;145.23363;12.92742;,
 4.97110;145.86732;12.92742;,
 4.97110;145.23363;13.17034;,
 4.97110;145.46732;13.17034;,
 4.97110;144.83363;12.92742;,
 -4.73901;145.46732;13.17034;,
 -4.73901;145.23363;13.17034;,
 -4.73901;145.86732;12.92742;,
 -5.13902;145.23363;12.92742;,
 -5.13902;145.46732;12.92742;,
 -4.73901;144.83363;12.92742;,
 -3.95666;144.83363;10.33088;,
 4.18874;144.83363;10.33088;,
 -3.56549;144.83363;5.23037;,
 3.79756;144.83363;5.23037;;
 
 55;
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
 4;22,19,18,23;,
 4;24,20,19,25;,
 4;26,21,20,27;,
 4;28,18,21,29;,
 3;28,23,18;,
 3;22,25,19;,
 3;24,27,20;,
 3;26,29,21;,
 4;25,30,31,24;,
 4;32,30,25,22;,
 4;33,31,30,34;,
 3;32,34,30;,
 3;33,35,31;,
 4;34,36,37,33;,
 4;38,36,34,32;,
 4;39,37,36,40;,
 4;35,33,37,41;,
 3;38,40,36;,
 3;39,41,37;,
 4;40,28,29,39;,
 4;23,28,40,38;,
 4;38,32,22,23;,
 3;42,41,39;,
 3;42,39,29;,
 4;35,41,42,43;,
 4;35,43,24,31;,
 4;44,45,43,42;,
 4;27,45,44,26;,
 4;26,44,42,29;,
 4;43,45,27,24;;
 
 MeshMaterialList {
  2;
  55;
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
  0,
  0,
  0,
  1,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\MATERIAL\\metal_00003.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;0.800000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
}

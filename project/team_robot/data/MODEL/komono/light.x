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
 32;
 -16.46668;71.87138;-16.38477;,
 16.46668;71.87138;-16.38477;,
 16.46668;69.64313;-16.38477;,
 -16.46668;69.64313;-16.38477;,
 16.46668;71.87138;16.38477;,
 16.46668;69.64313;16.38477;,
 -16.46668;71.87138;16.38477;,
 -16.46668;69.64313;16.38477;,
 -12.46809;69.51815;-12.62332;,
 12.91828;69.51815;-12.62332;,
 12.91828;67.67211;-12.62332;,
 -12.46809;67.67211;-12.62332;,
 12.91828;69.91815;-12.10489;,
 -12.46809;69.91815;-12.10489;,
 13.44717;67.67211;-12.10489;,
 13.44717;69.51815;-12.10489;,
 -12.46809;67.27211;-12.10489;,
 12.91828;67.27211;-12.10489;,
 -12.99698;69.51815;-12.10489;,
 -12.99698;67.67211;-12.10489;,
 13.44717;69.51815;12.78004;,
 13.44717;67.67211;12.78004;,
 12.91828;69.91815;12.78004;,
 12.91828;67.67211;13.29848;,
 12.91828;69.51815;13.29848;,
 12.91828;67.27211;12.78004;,
 -12.46809;69.51815;13.29848;,
 -12.46809;67.67211;13.29848;,
 -12.46809;69.91815;12.78004;,
 -12.99698;67.67211;12.78004;,
 -12.99698;69.51815;12.78004;,
 -12.46809;67.27211;12.78004;;
 
 32;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,0,3,7;,
 4;6,4,1,0;,
 4;3,2,5,7;,
 4;8,9,10,11;,
 4;12,9,8,13;,
 4;14,10,9,15;,
 4;16,11,10,17;,
 4;18,8,11,19;,
 3;18,13,8;,
 3;12,15,9;,
 3;14,17,10;,
 3;16,19,11;,
 4;15,20,21,14;,
 4;22,20,15,12;,
 4;23,21,20,24;,
 4;17,14,21,25;,
 3;22,24,20;,
 3;23,25,21;,
 4;24,26,27,23;,
 4;28,26,24,22;,
 4;29,27,26,30;,
 4;25,23,27,31;,
 3;28,30,26;,
 3;29,31,27;,
 4;30,18,19,29;,
 4;13,18,30,28;,
 4;31,29,19,16;,
 4;28,22,12,13;,
 4;16,17,25,31;;
 
 MeshMaterialList {
  2;
  32;
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
  1,
  1,
  1;;
  Material {
   0.392000;0.238400;0.097600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.600000;0.600000;0.600000;;
   0.600000;0.600000;0.600000;;
  }
 }
}

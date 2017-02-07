
PUBLIC int StringtoImg(char* imgname, char* name) {

	FILE *fp;

	if ((fp = fopen(imgname, "r")) == NULL ) {
		printf("error with %s file! \n", imgname);
		return EXIT_FAILURE;

	}
	BITMAPFILEHEADER *F;
	F = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (F == NULL ) {
		printf("Memory Allocation Failed!\n");
		return EXIT_FAILURE;
	}

	BITMAPINFOHEADER *I;
	I = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (I == NULL ) {
		printf("Memory Allocation Failed!\n");
		return EXIT_FAILURE;
	}

	fread(F, sizeof(BITMAPFILEHEADER), 1, fp);

	fread(I, sizeof(BITMAPINFOHEADER), 1, fp);
	int h = I->biHeight;
	int w = (I->biWidth) * 3;

	int i = 0;
	int j = 0;

	byte image[h][w];

	fread(image, h * w * sizeof(byte), 1, fp);
	fclose(fp);

	int size = getSize(name);

	char* txt = (char*) malloc(size);

	if ((fp = fopen(name, "r")) == NULL ) {
		printf("error with %s file! \n", name);
		return EXIT_FAILURE;
	}

	char c;
	char *p = txt;
	while ((c = fgetc(fp)) != EOF) {

		(*p) = c;
		p++;

	}

	fclose(fp);

	byte bits[h][w / 3]; //pixel

	for (i = 0; i < w / 3; i++) {

		for (j = 0; j < h; j++) {

			bits[j][i] = 128 * getBit(txt, h * i + j);

		}

	}
	byte img[h][w];

	for (i = 0; i < h; i++) {

		for (j = 0; j < w / 3; j++) {

			img[i][3 * j] = bits[i][j];
			img[i][3 * j + 1] = bits[i][j];
			img[i][3 * j + 2] = bits[i][j];

		}

	}

	FILE *fp1;

	int size1 = strlen(imgname);
	char *newfilename = (char*) malloc(size1 + 11);
	strcpy(newfilename, "new-");
	strcat(newfilename, imgname);
	fp1 = fopen(newfilename, "wb");

	fwrite(F, sizeof(BITMAPFILEHEADER), 1, fp1);
	free(F);

	fwrite(I, sizeof(BITMAPINFOHEADER), 1, fp1);
	free(I);

	for (i = 0; i < h; i++)
		fwrite(img[i], sizeof(byte) * w, 1, fp1);

	fclose(fp1);

	printf("%s created!\n", newfilename);
	return EXIT_SUCCESS;
}

PUBLIC int decodeimgtostring(char* name) {

	FILE *fp;

	if ((fp = fopen(name, "r")) == NULL ) {
		printf("Error with %s file!\n", name);
		return EXIT_FAILURE;
	}

	BITMAPFILEHEADER *F;
	F = (BITMAPFILEHEADER*) malloc(sizeof(BITMAPFILEHEADER));
	if (F == NULL ) {
		printf("Memory allocation failed!\n");
		return EXIT_FAILURE;

	}

	BITMAPINFOHEADER *I;
	I = (BITMAPINFOHEADER*) malloc(sizeof(BITMAPINFOHEADER));
	if (I == NULL ) {
		printf("Memory allocation failed!\n");
		return EXIT_FAILURE;

	}

	fread(F, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(I, sizeof(BITMAPINFOHEADER), 1, fp);

	int h = I->biHeight;
	int w = (I->biWidth) * 3;

	byte Image[h][w];
	fread(Image, h * w * sizeof(byte), 1, fp);
	fclose(fp);

	byte bits[h][w / 3];
	int i = 0;
	int j = 0;
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			bits[i][j] = Image[i][j * 3] / 128;
		}
	}

	byte test2[h * (w / 3)];
	int m = 0;
	for (i = 0; i < w / 3; i++)
		for (j = 0; j < h; j++) {
			test2[m] = bits[j][i];
			m++;
		}

	byte b;
	byte k = 0;
	char *s = (char*) calloc(h * w, 1);
	char* q = s;
	int count = 1;
	for (i = 0; i < h * (w / 3); i++) {

		k = k << 1;
		b = test2[i];
		k = (k | b);

		count++;

		if (count == 9) {
			(*q) = k;
			q++;
			k = 0;
			count = 1;
		}

	}

	if ((fp = fopen("outputText.txt", "wb")) == NULL ) {
		printf("Error with %s file!\n", name);
		return EXIT_FAILURE;
	}

	fputs(s, fp);
	printf("outputText.txt created!\n");
	fclose(fp);

	return EXIT_SUCCESS;
}

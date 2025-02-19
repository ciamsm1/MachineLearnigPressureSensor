#include <SoftwareSerial.h>
#define BT_RXD 0
#define BT_TXD 1
SoftwareSerial bluetooth(BT_RXD, BT_TXD);

const int analogPin = A0;
const int height = 8;  // 행렬의 높이
const int width = 16;  // 행렬의 너비
const int newHeight = 16;  // 업스케일링된 행렬의 높이
const int newWidth = 32;  // 업스케일링된 행렬의 너비

int MaskedMatrix[8][16];
float FirstMatrix[8][16];
float ProcessedMatrix[8][16];
float UpscaledMatrix[16][32];
float weights[16][32] = 
{
    {-0.050648, -0.0460942, -0.0415404, -0.0463432, -0.0518143, -0.0351651, -0.0151128, -0.00547228, 0.0015653, 0.00297741, 0.00234391, 0.00629273, 0.0125327, 0.0233371, 0.0371844, 0.0461376, 
     0.00884942, 0.00879065, -0.0000396509, -0.0092539, -0.0118633, -0.00480793, 0.00443999, 0.00859989, 0.000642508, -0.00708501, -0.00571828, -0.00864861, -0.0325476, -0.0497936, -0.0521521, -0.0612764},
    {-0.00303324, -0.00251954, -0.00251615, -0.00273878, -0.00230557, -0.00523023, -0.0083075, -0.00524422, -0.00757441, -0.00701656, 0.000986708, -0.000442019, -0.02176, -0.0353217, -0.0359454, -0.0452065, 
     -0.0152053, -0.0124533, -0.00195052, 0.00544916, 0.00733887, 0.00146922, -0.00557182, -0.00522024, -0.0060324, -0.00360967, 0.00376914, 0.0051241, -0.00805931, -0.0163517, -0.0163013, -0.0212882},
    {-0.0103783, -0.00519715, 0.00538407, 0.0135071, 0.0159601, 0.00757708, -0.00299906, -0.00631947, -0.00623681, -0.00325874, 0.000841459, 0.00367625, -0.00244249, -0.00693768, -0.00449787, -0.00258327, 
     -0.0042756, 0.00239111, 0.0103384, 0.0156904, 0.0161157, 0.00676952, -0.00518889, -0.0116466, -0.010425, -0.00433241, -0.000292339, 0.0037095, 0.0025035, 0.00146962, 0.00774969, 0.0146817},
    {-0.00426977, 0.00111216, 0.0067626, 0.00972399, 0.00939695, 0.00301818, -0.00415353, -0.0111573, -0.0143799, -0.00888724, -0.00458603, 0.000129262, 0.00679129, 0.0120638, 0.0140307, 0.0164926, 
     -0.00850647, -0.00472386, -0.000441864, 0.00181665, 0.00283619, 0.000764455, 0.00033419, -0.00477702, -0.0127967, -0.0104597, -0.00791197, -0.00269088, 0.0140535, 0.026999, 0.0210839, 0.0161284},
    {-0.00929432, -0.00582029, -0.00188161, 0.0000911966, 0.00141654, 0.00151258, 0.00508785, 0.00126224, -0.00939778, -0.00987671, -0.00899439, -0.00378144, 0.0116266, 0.02186, 0.0173521, 0.0147343, 
     -0.00987659, -0.00594725, -0.00178721, 0.00021189, 0.00107612, -0.000666508, 0.0020887, 0.00132033, -0.00688559, -0.00695101, -0.00487632, -0.000416376, 0.00315612, 0.00236662, 0.00665253, 0.0133835},
    {-0.0124918, -0.00766979, -0.00218683, 0.00123223, 0.0025339, -0.00550012, -0.0111737, -0.0074712, -0.0067575, -0.002706, 0.0026674, 0.00644979, 0.00327893, -0.00285185, 0.00424451, 0.0138401, 
     -0.0124862, -0.0063055, 0.00185183, 0.00694474, 0.00817141, -0.0040022, -0.0186531, -0.0158202, -0.00899678, -0.00211849, 0.00502598, 0.00919137, 0.00967467, 0.00650565, 0.007242, 0.0109187},
    {-0.0151036, -0.00732523, 0.00337255, 0.00855823, 0.00815801, 0.00210073, -0.00772826, -0.00863841, -0.00515884, -0.000609893, 0.00454777, 0.00863013, 0.0149822, 0.0148337, 0.00870928, 0.00693405, 
     -0.0175154, -0.00887582, 0.00269368, 0.0067256, 0.00381352, 0.00473213, 0.00516997, 0.00529818, 0.00401963, 0.00347418, 0.00297762, 0.00315671, 0.0140677, 0.0171648, 0.00762254, 0.00302029},
    {-0.0152935, -0.00811628, 0.00191018, 0.00410029, -0.00105197, 0.00338781, 0.00976206, 0.0170276, 0.0195441, 0.0152281, 0.00848943, 0.000312961, 0.00723485, 0.0133741, 0.00741043, 0.00206991, 
     -0.0137121, -0.0109618, -0.00599034, -0.00564119, -0.00776354, 0.00448482, 0.0191174, 0.0354748, 0.0427307, 0.0367717, 0.0306508, 0.018815, 0.0123807, 0.0129316, 0.0122271, 0.00782896},
    {-0.0219762, -0.0202857, -0.0170021, -0.021073, -0.0231694, -0.00284553, 0.0208156, 0.0285559, 0.0182226, 0.00465288, -0.000868508, -0.00144097, 0.00116155, 0.00830168, 0.0182238, 0.0230848, 
     0.0111665, 0.0105197, 0.00101758, -0.00868279, -0.0116231, -0.00491111, 0.00383438, 0.00782723, -0.0000899303, -0.00789568, -0.00645582, -0.00909744, -0.0327555, -0.0498117, -0.0519091, -0.0609186},
    {-0.00193646, -0.00163932, -0.00189103, -0.00233978, -0.00197571, -0.00526294, -0.00889195, -0.00591478, -0.00815933, -0.00767344, 0.000340448, -0.000855283, -0.0218941, -0.03528, -0.0357188, -0.0449206, 
     -0.0146905, -0.0120879, -0.00172776, 0.00546447, 0.00730998, 0.00136796, -0.00588076, -0.00557386, -0.00640279, -0.00409843, 0.00321039, 0.00470125, -0.00821762, -0.0163666, -0.0162027, -0.0211065},
    {-0.0102272, -0.00504318, 0.00558315, 0.0134727, 0.0156665, 0.00739978, -0.00303301, -0.00639383, -0.00640833, -0.00355022, 0.000477959, 0.00340397, -0.0025592, -0.00702536, -0.00452948, -0.00248184, 
     -0.00436008, 0.00244795, 0.0106128, 0.0158975, 0.0160246, 0.00672527, -0.00504895, -0.0115133, -0.0103431, -0.00430591, -0.000291396, 0.00376401, 0.00256863, 0.00145343, 0.00777729, 0.0148561},
    {-0.00450707, 0.000978364, 0.00678271, 0.00987849, 0.0095261, 0.00318333, -0.0038855, -0.0108645, -0.0141009, -0.00862577, -0.00431595, 0.000404394, 0.00702125, 0.0121933, 0.0141046, 0.0166076, 
     -0.00882799, -0.00499891, -0.000669708, 0.00174486, 0.00292478, 0.00100433, 0.000697955, -0.00435585, -0.0123899, -0.0100712, -0.00751077, -0.00232553, 0.0143841, 0.0272735, 0.0211935, 0.0161644},
    {-0.00956966, -0.00604482, -0.00211152, -0.000122937, 0.00128632, 0.0016069, 0.00538765, 0.00165361, -0.00900316, -0.00947678, -0.00852202, -0.00332519, 0.0120108, 0.0221974, 0.0175255, 0.0148304, 
     -0.00998589, -0.00600859, -0.00191915, -0.0000232351, 0.000808551, -0.00076, 0.00219716, 0.00153023, -0.0066404, -0.00665229, -0.00439513, 0.000173426, 0.00368609, 0.00281039, 0.0069224, 0.0135644},
    {-0.0124842, -0.00765971, -0.00228117, 0.00103756, 0.00229842, -0.00566547, -0.011242, -0.00746039, -0.00669256, -0.00255244, 0.00308044, 0.00715641, 0.00405518, -0.00222331, 0.0045934, 0.0139961, 
     -0.0125717, -0.00641624, 0.00171933, 0.00679244, 0.0080177, -0.00415324, -0.018808, -0.0159241, -0.00903366, -0.0020536, 0.00537303, 0.00993094, 0.0105254, 0.0071362, 0.00755307, 0.0109934},
    {-0.0153593, -0.00756167, 0.00322348, 0.00846172, 0.00811057, 0.00201874, -0.007871, -0.00874741, -0.0052009, -0.000582136, 0.00478558, 0.00919808, 0.0156194, 0.0152748, 0.00890586, 0.00695171, 
     -0.0177883, -0.00907591, 0.00258436, 0.00667471, 0.00385272, 0.00472719, 0.00510737, 0.00527902, 0.00404706, 0.00351043, 0.00302463, 0.00337482, 0.0145099, 0.0176235, 0.00784026, 0.00303027},
    {-0.0154666, -0.00824401, 0.00180821, 0.00399812, -0.00109282, 0.0033547, 0.00974912, 0.0170419, 0.0195463, 0.0152493, 0.00845229, 0.0003148, 0.00753993, 0.0139018, 0.00778646, 0.00220577, 
     -0.0137691, -0.0110359, -0.00606352, -0.0057564, -0.00786447, 0.00440244, 0.0190463, 0.0353584, 0.0425704, 0.0366991, 0.0306359, 0.0188414, 0.0125925, 0.0133959, 0.012744, 0.00818814},
};

int readMux(int channel)  
{ 
    int controlPin[] = {8, 9, 10, 11}; 
    int muxChannel[16][4] = { 
        {0,0,0,0},  
        {1,0,0,0}, //channel 1 
        {0,1,0,0}, //channel 2 
        {1,1,0,0}, //channel 3 
        {0,0,1,0}, //channel 4 
        {1,0,1,0}, //channel 5 
        {0,1,1,0}, //channel 6 
        {1,1,1,0}, //channel 7 
        {0,0,0,1}, //channel 8 
        {1,0,0,1}, //channel 9 
        {0,1,0,1}, //channel 10 
        {1,1,0,1}, //channel 11 
        {0,0,1,1}, //channel 12 
        {1,0,1,1}, //channel 13 
        {0,1,1,1}, //channel 14 
        {1,1,1,1}  //channel 15 
    }; 
  
    for(int i = 0; i < 4; i ++) { 
        digitalWrite(controlPin[i], muxChannel[channel][i]); 
    } 
    int val = analogRead(analogPin); 
    return val; 
}

float predict(float weights[16][32], float x[16][32]) 
{
    float z = 0.0;
    for (int i = 0; i < newHeight; ++i) 
    { 
        for (int j = 0; j < newWidth; ++j) 
        {
            z += weights[i][j] * x[i][j];
        }
    }
    return 1.0 / (1.0 + exp(-z));
}

float calculateX(float matrix[8][16], int i, int j) 
{
    float sum = 0.0f;
    int count = 0;

    // 8방향 이웃 탐색 (상, 하, 좌, 우, 대각선)
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;  // 자기 자신은 제외
            int ni = i + di;
            int nj = j + dj;
            if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                sum += matrix[ni][nj];
                ++count;
            }
        }
    }

    if (count > 0) {
        return sum / count;  // 이웃 값들의 평균
    } else {
        return matrix[i][j];  // 이웃이 없으면 자기 자신의 값 반환
    }
}

// 선형 보간 함수
float linearInterpolate(float v0, float v1, float t) {
    return v0 * (1 - t) + v1 * t;
}

void processMatrix(float array[8][16], int Mask[8][16], float result[8][16]) 
{
    float sum = 0.0f, sum_sq = 0.0f;

    for (int i = 0; i < height; ++i) 
    {
        for (int j = 0; j < width; ++j) 
        {
            result[i][j] = array[i][j];
            sum += result[i][j];
            sum_sq += result[i][j] * result[i][j];
        }
    }

    int n = width * height;
    float mean = sum / n;
    float std_dev = sqrt((sum_sq / n) - (mean * mean));
    float Thup = mean + std_dev;
    float Thdn = mean - std_dev;

    for (int i = 0; i < height; ++i) 
    {
        for (int j = 0; j < width; ++j) 
        {
            if (result[i][j] < Thup && result[i][j] > Thdn) 
            {
                Mask[i][j] = 1;
            } else {
                Mask[i][j] = 0;
            }
        }
    }
  
    for (int i = 0; i < height; ++i) 
    {
        for (int j = 0; j < width; ++j) 
        {
            if (Mask[i][j] == 0) 
            {
                result[i][j] = result[i][j];
            } else 
            {
                result[i][j] = calculateX(result, i, j);
            }
        }  
    }
}

// 업스케일링 함수
void upscaleMatrix(float inputMatrix[8][16], float outputMatrix[16][32]) 
{
    for (int i = 0; i < newHeight; ++i) 
    {
        for (int j = 0; j < newWidth; ++j) 
        {
            float x = static_cast<float>(j) / (newWidth - 1) * (width - 1);
            float y = static_cast<float>(i) / (newHeight - 1) * (height - 1);
            
            int x0 = static_cast<int>(x);
            int y0 = static_cast<int>(y);

            int x1 = min(x0 + 1, width - 1);
            int y1 = min(y0 + 1, height - 1);

            float xFraction = x - x0;
            float yFraction = y - y0;

            float top = linearInterpolate(inputMatrix[y0][x0], inputMatrix[y0][x1], xFraction);
            float bottom = linearInterpolate(inputMatrix[y1][x0], inputMatrix[y1][x1], xFraction);
            outputMatrix[i][j] = linearInterpolate(top, bottom, yFraction);
        }
    }
}

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
}

void loop() 
{
  for (int i = 0; i < height; ++i)
  {
    digitalWrite(2, LOW);
    shiftOut(4, 3, LSBFIRST, 1 << i);  
    digitalWrite(2, HIGH);
    
    for(int j = 0; j < width; ++j)
    { 
     FirstMatrix[i][j] = readMux(j);
    } 
    delay(1);
  }
  
  processMatrix(FirstMatrix, MaskedMatrix, ProcessedMatrix);
  upscaleMatrix(ProcessedMatrix, UpscaledMatrix);

  if(predict(weights, UpscaledMatrix) < 0.5 )
  {
    digitalWrite(5,HIGH);
    digitalWrite(6,LOW);
    Serial.println("false");
  }else 
  {
    digitalWrite(6,HIGH);
    digitalWrite(5,LOW);
    Serial.println("true");
  }

  delay(100);
}

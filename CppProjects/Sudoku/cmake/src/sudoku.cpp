#include "include/sudoku.h"
#include <algorithm>
#include <cmath>
#include <vector>

const int sizex = 20;
const int sizey = 30;

void drawLine(Vec2f line, Mat &img, Scalar rgb = CV_RGB(0,0,255)) {
    if(line[1]!=0) {
       float m = -1/tan(line[1]);
       float c = line[0]/sin(line[1]);
       cv::line(img, Point(0, c), Point(img.size().width, m*img.size().width+c), rgb);
       } else {
       cv::line(img, Point(line[0], 0), Point(line[0], img.size().height), rgb);
     }
}

void mergeRelatedLines(vector<Vec2f> *lines, Mat &img) {
     vector<Vec2f>::iterator current;
     for(current=lines->begin();current!=lines->end();current++) {
        if((*current)[0]==0 && (*current)[1]==-100) continue;
        float p1 = (*current)[0];
        float theta1 = (*current)[1];
        Point pt1current, pt2current;
        if(theta1>CV_PI*45/180 && theta1<CV_PI*135/180) {
            pt1current.x=0; pt1current.y = p1/sin(theta1);
            pt2current.x=img.size().width;
            pt2current.y=-pt2current.x/tan(theta1) + p1/sin(theta1);
            } else {
            pt1current.y=0;
            pt1current.x=p1/cos(theta1);
            pt2current.y=img.size().height;
            pt2current.x=-pt2current.y/tan(theta1) + p1/cos(theta1);
        }
        vector<Vec2f>::iterator pos;
        for(pos=lines->begin();pos!=lines->end();pos++) {
            if(*current==*pos) continue;
            if(fabs((*pos)[0]-(*current)[0])<20 && fabs((*pos)[1]-(*current)[1])<CV_PI*10/180) {
                float p = (*pos)[0];
                float theta = (*pos)[1];
                Point pt1, pt2;
                if((*pos)[1]>CV_PI*45/180 && (*pos)[1]<CV_PI*135/180) {
                    pt1.x=0; pt1.y = p/sin(theta);
                    pt2.x=img.size().width;
                    pt2.y=-pt2.x/tan(theta) + p/sin(theta);
                }else{
                    pt1.y=0;
                    pt1.x=p/cos(theta);
                    pt2.y=img.size().height; pt2.x=-pt2.y/tan(theta) + p/cos(theta);
                }
                if(((double)(pt1.x-pt1current.x)*(pt1.x-pt1current.x) + (pt1.y-pt1current.y)*(pt1.y-pt1current.y)<64*64) && ((double)(pt2.x-pt2current.x)*(pt2.x-pt2current.x) + (pt2.y-pt2current.y)*(pt2.y-pt2current.y)<64*64)) {
                 // Merge the two
                 (*current)[0] = ((*current)[0]+(*pos)[0])/2;
                 (*current)[1] = ((*current)[1]+(*pos)[1])/2;
                 (*pos)[0]=0; (*pos)[1]=-100;
                }
             }
         }
     }
}

static bool findBoardQuadByContours(const Mat& gray, Point2f outQuad[4]) {
    Mat blurred;
    GaussianBlur(gray, blurred, Size(11, 11), 0);

    double imgArea = (double)gray.rows * (double)gray.cols;
    double bestArea = 0.0;
    bool found = false;
    const double epsilons[] = {0.015, 0.02, 0.03, 0.04, 0.05};
    const int blockSizes[] = {5, 11};
    Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);

    for (int blockSize : blockSizes) {
        Mat threshed, dilated;
        adaptiveThreshold(blurred, threshed, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, blockSize, 2);
        dilate(threshed, dilated, kernel);

        vector<vector<Point>> contours;
        findContours(dilated, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

        for (size_t i = 0; i < contours.size(); i++) {
            double cntArea = contourArea(contours[i]);
            if (cntArea < 0.08 * imgArea || cntArea > 0.95 * imgArea)
                continue;

            double peri = arcLength(contours[i], true);
            for (double eps : epsilons) {
                vector<Point> approx;
                approxPolyDP(contours[i], approx, eps * peri, true);
                if (approx.size() == 4 && isContourConvex(approx)) {
                    double quadArea = contourArea(approx);
                    if (quadArea < 0.08 * imgArea || quadArea > 0.95 * imgArea)
                        continue;

                    // Order corners: top-left, top-right, bottom-right, bottom-left
                    int tlIdx = 0, brIdx = 0, trIdx = 0, blIdx = 0;
                    double minSum = 1e18, maxSum = -1e18, minDiff = 1e18, maxDiff = -1e18;
                    for (int k = 0; k < 4; k++) {
                        double s = approx[k].x + approx[k].y;
                        double d = approx[k].y - approx[k].x;
                        if (s < minSum) { minSum = s; tlIdx = k; }
                        if (s > maxSum) { maxSum = s; brIdx = k; }
                        if (d < minDiff) { minDiff = d; trIdx = k; }
                        if (d > maxDiff) { maxDiff = d; blIdx = k; }
                    }

                    // Ensure all 4 corner indices are distinct
                    int idxMask = (1 << tlIdx) | (1 << trIdx) | (1 << brIdx) | (1 << blIdx);
                    if (idxMask != 0x0F)
                        continue;

                    Point2f quad[4] = {
                        Point2f((float)approx[tlIdx].x, (float)approx[tlIdx].y),
                        Point2f((float)approx[trIdx].x, (float)approx[trIdx].y),
                        Point2f((float)approx[brIdx].x, (float)approx[brIdx].y),
                        Point2f((float)approx[blIdx].x, (float)approx[blIdx].y)
                    };

                    double w1 = norm(quad[1] - quad[0]);
                    double w2 = norm(quad[2] - quad[3]);
                    double h1 = norm(quad[3] - quad[0]);
                    double h2 = norm(quad[2] - quad[1]);

                    // Opposite sides must have similar lengths (perspective foreshortening tolerance)
                    double widthSym = std::min(w1, w2) / std::max(1.0, std::max(w1, w2));
                    double heightSym = std::min(h1, h2) / std::max(1.0, std::max(h1, h2));
                    if (widthSym < 0.75 || heightSym < 0.75)
                        continue;

                    // Interior angles must be reasonably close to 90 degrees (|cos| < 0.28)
                    bool anglesOk = true;
                    for (int k = 0; k < 4; k++) {
                        Point2f v1 = quad[(k + 1) % 4] - quad[k];
                        Point2f v2 = quad[(k + 3) % 4] - quad[k];
                        double cosAngle = std::abs(v1.dot(v2)) / std::max(1.0, norm(v1) * norm(v2));
                        if (cosAngle > 0.28) {
                            anglesOk = false;
                            break;
                        }
                    }
                    if (!anglesOk)
                        continue;

                    double avgW = 0.5 * (w1 + w2);
                    double avgH = std::max(1.0, 0.5 * (h1 + h2));
                    double ratio = avgW / avgH;

                    if (ratio >= 0.75 && ratio <= 1.35 && quadArea > bestArea) {
                        bestArea = quadArea;
                        for (int k = 0; k < 4; k++) outQuad[k] = quad[k];
                        found = true;
                    }
                }
            }
        }
        if (found) break;
    }
    return found;
}

int getBox(Mat& image, Mat& box, Mat& Originalbox, Mat* outInvTransform,
           Point2f* ioSmoothedQuad, bool* ioHasPrevQuad, bool allowHoughFallback,
           Mat* outColorWarped)
{
    Mat sudoku;
    cv::cvtColor(image, sudoku, COLOR_BGR2GRAY);
    Mat sudoku_original = sudoku.clone();

    Point2f src[4];
    bool quadFound = findBoardQuadByContours(sudoku_original, src);

    if (!quadFound) {
        if (!allowHoughFallback) {
            return 0;
        }
        // Fallback: FloodFill + HoughLines extreme-edge detector
        Mat outerBox = Mat(sudoku.size(), CV_8UC1);
        GaussianBlur(sudoku, sudoku, Size(11,11), 0);
        adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);
        bitwise_not(outerBox, outerBox);
        Mat kernel = (Mat_<uchar>(3,3) << 0,1,0,1,1,1,0,1,0);
        dilate(outerBox, outerBox, kernel);

        int maxArea = -1;
        Point maxPt(0, 0);
        for(int y=0;y<outerBox.size().height;y++) {
            uchar *row = outerBox.ptr(y);
            for(int x=0;x<outerBox.size().width;x++) {
                if(row[x]>=128) {
                    int area = floodFill(outerBox, Point(x,y), CV_RGB(0,0,64));
                    if(area>maxArea) {
                        maxPt = Point(x,y);
                        maxArea = area;
                    }
                }
            }
        }

        if (maxArea <= 0) return 0;

        floodFill(outerBox, maxPt, CV_RGB(255,255,255));
        for(int y=0;y<outerBox.size().height;y++) {
            uchar *row = outerBox.ptr(y);
            for(int x=0;x<outerBox.size().width;x++) {
                if(row[x]==64 && x!=maxPt.x && y!=maxPt.y) {
                    floodFill(outerBox, Point(x,y), CV_RGB(0,0,0));
                }
            }
        }

        erode(outerBox, outerBox, kernel);

        vector<Vec2f> lines;
        HoughLines(outerBox, lines, 1, CV_PI/180, 200);
        if (lines.empty()) return 0;
        mergeRelatedLines(&lines, sudoku);

        Vec2f topEdge = Vec2f(1000,1000);
        Vec2f bottomEdge = Vec2f(-1000,-1000);
        Vec2f leftEdge = Vec2f(1000,1000);
        double leftXIntercept = 100000;
        Vec2f rightEdge = Vec2f(-1000,-1000);
        double rightXIntercept = 0;

        for(size_t i=0;i<lines.size();i++){
            Vec2f current = lines[i];
            float p=current[0];
            float theta=current[1];
            if(p==0 && theta==-100)
                continue;

            double xIntercept = p/cos(theta);

            if(theta>CV_PI*80/180 && theta<CV_PI*100/180){
                if(p<topEdge[0])
                    topEdge = current;
                if(p>bottomEdge[0])
                    bottomEdge = current;
            }
            else if(theta<CV_PI*10/180 || theta>CV_PI*170/180){
                if(xIntercept>rightXIntercept){
                    rightEdge = current;
                    rightXIntercept = xIntercept;
                }else if(xIntercept<=leftXIntercept){
                    leftEdge = current;
                    leftXIntercept = xIntercept;
                }
            }
        }

        if (topEdge[0] == 1000 || bottomEdge[0] == -1000 || leftEdge[0] == 1000 || rightEdge[0] == -1000) {
            return 0;
        }

        Point left1, left2, right1, right2, bottom1, bottom2, top1, top2;
        int height=outerBox.size().height;
        int width=outerBox.size().width;
        if(leftEdge[1]!=0){
            left1.x=0;
            left1.y=leftEdge[0]/sin(leftEdge[1]);
            left2.x=width;
            left2.y=-left2.x/tan(leftEdge[1]) + left1.y;
        }else{
            left1.y=0;
            left1.x=leftEdge[0]/cos(leftEdge[1]);
            left2.y=height;
            left2.x=left1.x - height*tan(leftEdge[1]);
        }

        if(rightEdge[1]!=0){
            right1.x=0;
            right1.y=rightEdge[0]/sin(rightEdge[1]);
            right2.x=width;
            right2.y=-right2.x/tan(rightEdge[1]) + right1.y;
        }else{
            right1.y=0;
            right1.x=rightEdge[0]/cos(rightEdge[1]);
            right2.y=height;
            right2.x=right1.x - height*tan(rightEdge[1]);
        }
        bottom1.x=0;
        bottom1.y=bottomEdge[0]/sin(bottomEdge[1]);
        bottom2.x=width;
        bottom2.y=-bottom2.x/tan(bottomEdge[1]) + bottom1.y;
        top1.x=0;
        top1.y=topEdge[0]/sin(topEdge[1]);
        top2.x=width;
        top2.y=-top2.x/tan(topEdge[1]) + top1.y;

        double leftA = left2.y-left1.y;
        double leftB = left1.x-left2.x;
        double leftC = leftA*left1.x + leftB*left1.y;
        double rightA = right2.y-right1.y;
        double rightB = right1.x-right2.x;
        double rightC = rightA*right1.x + rightB*right1.y;
        double topA = top2.y-top1.y;
        double topB = top1.x-top2.x;
        double topC = topA*top1.x + topB*top1.y;
        double bottomA = bottom2.y-bottom1.y;
        double bottomB = bottom1.x-bottom2.x;
        double bottomC = bottomA*bottom1.x + bottomB*bottom1.y;

        double detTopLeft = leftA*topB - leftB*topA;
        double detTopRight = rightA*topB - rightB*topA;
        double detBottomRight = rightA*bottomB - rightB*bottomA;
        double detBottomLeft = leftA*bottomB-leftB*bottomA;
        if (std::abs(detTopLeft) < 1e-6 || std::abs(detTopRight) < 1e-6 ||
            std::abs(detBottomRight) < 1e-6 || std::abs(detBottomLeft) < 1e-6) {
            return 0;
        }

        Point ptTopLeft = Point((topB*leftC - leftB*topC)/detTopLeft, (leftA*topC - topA*leftC)/detTopLeft);
        Point ptTopRight = Point((topB*rightC-rightB*topC)/detTopRight, (rightA*topC-topA*rightC)/detTopRight);
        Point ptBottomRight = Point((bottomB*rightC-rightB*bottomC)/detBottomRight, (rightA*bottomC-bottomA*rightC)/detBottomRight);
        Point ptBottomLeft = Point((bottomB*leftC-leftB*bottomC)/detBottomLeft, (leftA*bottomC-bottomA*leftC)/detBottomLeft);

        src[0] = ptTopLeft;
        src[1] = ptTopRight;
        src[2] = ptBottomRight;
        src[3] = ptBottomLeft;
    }

    // Apply temporal smoothing on quad corners if tracking in camera mode
    if (ioSmoothedQuad && ioHasPrevQuad) {
        if (*ioHasPrevQuad) {
            double avgShift = 0.0;
            for (int k = 0; k < 4; k++) {
                avgShift += norm(src[k] - ioSmoothedQuad[k]);
            }
            avgShift /= 4.0;
            if (avgShift < 60.0) {
                const float alpha = 0.60f;
                for (int k = 0; k < 4; k++) {
                    src[k] = src[k] * alpha + ioSmoothedQuad[k] * (1.0f - alpha);
                    ioSmoothedQuad[k] = src[k];
                }
            } else {
                for (int k = 0; k < 4; k++) ioSmoothedQuad[k] = src[k];
            }
        } else {
            for (int k = 0; k < 4; k++) ioSmoothedQuad[k] = src[k];
            *ioHasPrevQuad = true;
        }
    } else if (ioSmoothedQuad) {
        for (int k = 0; k < 4; k++) ioSmoothedQuad[k] = src[k];
    }

    double maxLenD = 0.0;
    for (int i = 0; i < 4; i++) {
        double d = norm(src[i] - src[(i + 1) % 4]);
        if (d > maxLenD) maxLenD = d;
    }
    int maxLength = std::max(9, (int)std::ceil(maxLenD));

    Point2f dst[4];
    dst[0] = Point2f(0, 0);
    dst[1] = Point2f((float)(maxLength - 1), 0);
    dst[2] = Point2f((float)(maxLength - 1), (float)(maxLength - 1));
    dst[3] = Point2f(0, (float)(maxLength - 1));

    Mat H = cv::getPerspectiveTransform(src, dst);
    if (outInvTransform) {
        *outInvTransform = cv::getPerspectiveTransform(dst, src);
    }
    if (outColorWarped) {
        cv::warpPerspective(image, *outColorWarped, H, Size(maxLength, maxLength));
    }

    // Draw detected board perimeter on original image using E2i corporate colors (#244B5A + #99E6D8)
    for (int i = 0; i < 4; i++) {
        line(image, src[i], src[(i + 1) % 4], E2I_PRIMARY_DARK, 4, LINE_AA);
        line(image, src[i], src[(i + 1) % 4], E2I_PRIMARY_MINT, 2, LINE_AA);
    }

    Mat undistorted = Mat(Size(maxLength, maxLength), CV_8UC1);
    Mat sudoku_gaus;
    GaussianBlur(sudoku_original, sudoku_gaus, Size(9, 9), 0);
    cv::warpPerspective(sudoku_gaus, undistorted, H, Size(maxLength, maxLength));
    box = undistorted.clone();

    Mat ohneGaus;
    cv::warpPerspective(sudoku_original, ohneGaus, H, Size(maxLength, maxLength));
    Originalbox = ohneGaus.clone();
    return maxLength;
}

int extractSudokuDigits(const Mat& undistorted, const Mat& ohneGaus, int maxLength,
                        DigitRecognizer& dr, Mat& sudokuMat, Mat& altSudokuMat,
                        Mat& confidenceMat, bool showRoiGui)
{
    Mat undistortedThreshed = undistorted.clone();
    adaptiveThreshold(undistorted, undistortedThreshed, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 21, 2);

    int dist = (int)std::ceil((double)maxLength / 9.0);
    sudokuMat = cv::Mat::zeros(cv::Size(9, 9), CV_8UC1);
    altSudokuMat = cv::Mat::zeros(cv::Size(9, 9), CV_8UC1);
    confidenceMat = cv::Mat::zeros(cv::Size(9, 9), CV_32FC1);

    int detectedCount = 0;

    for (int j = 0; j < 9; j++) {
        for (int i = 0; i < 9; i++) {
            int y0 = std::min(j * dist, maxLength - 1);
            int y1 = std::min((j + 1) * dist, maxLength);
            int x0 = std::min(i * dist, maxLength - 1);
            int x1 = std::min((i + 1) * dist, maxLength);
            if (x1 <= x0 || y1 <= y0) continue;

            Rect cellRect(x0, y0, x1 - x0, y1 - y0);
            Mat currentCell = undistortedThreshed(cellRect).clone();
            Mat numberCell = ohneGaus(cellRect).clone();

            int ch = currentCell.rows;
            int cw = currentCell.cols;

            Moments m = cv::moments(currentCell, true);
            int area = (int)m.m00;

            if (area > ch * cw / 10) {
                vector<vector<Point>> contours;
                Mat currentCellCopy = currentCell.clone();
                findContours(currentCellCopy, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

                double bestScore = -1e9;
                Rect bestRect;
                bool foundDigit = false;
                double minCntArea = std::max(20.0, (ch * cw) * 0.025);

                for (size_t k = 0; k < contours.size(); k++) {
                    double cntArea = contourArea(contours[k]);
                    Rect rec = boundingRect(contours[k]);

                    if (cntArea > minCntArea &&
                        rec.height > ch / 3 && rec.height < (int)(ch * 0.92) &&
                        rec.width > cw / 6 && rec.width < (cw * 2) / 3) {

                        double cx = rec.x + rec.width / 2.0;
                        double cy = rec.y + rec.height / 2.0;
                        double distCenter = std::hypot(cx - cw / 2.0, cy - ch / 2.0);

                        if (distCenter < ch * 0.35) {
                            double score = cntArea - distCenter * 3.0;
                            if (score > bestScore) {
                                bestScore = score;
                                bestRect = rec;
                                foundDigit = true;
                            }
                        }
                    }
                }

                if (foundDigit) {
                    Mat roi = numberCell(bestRect);

                    Mat thresholdImage;
                    adaptiveThreshold(roi, thresholdImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 11, 2);

                    Mat outImage;
                    resize(thresholdImage, outImage, Size(sizex, sizey));

                    if (showRoiGui) {
                        imshow("roi", outImage);
                        waitKey(10);
                    }

                    int secondBest = 0;
                    float margin = 0.0f;
                    int number = dr.classify(outImage, &secondBest, &margin);

                    if (number >= 1 && number <= 9) {
                        sudokuMat.at<uchar>(j, i) = (uchar)number;
                        altSudokuMat.at<uchar>(j, i) = (uchar)secondBest;
                        confidenceMat.at<float>(j, i) = margin;
                        detectedCount++;
                    }
                }
            }
        }
    }
    return detectedCount;
}

static bool solveSudokuRecursive(int grid[9][9], int rowMask[9], int colMask[9], int boxMask[9]) {
    int bestR = -1, bestC = -1, minOptions = 10, bestMask = 0;

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (grid[r][c] == 0) {
                int b = (r / 3) * 3 + (c / 3);
                int used = rowMask[r] | colMask[c] | boxMask[b];
                int avail = (~used) & 0x3FE; // bits 1..9
                int count = 0;
                for (int m = avail; m; m &= (m - 1)) count++;
                if (count == 0) return false;
                if (count < minOptions) {
                    minOptions = count;
                    bestR = r;
                    bestC = c;
                    bestMask = avail;
                    if (count == 1) break;
                }
            }
        }
        if (minOptions == 1) break;
    }

    if (bestR == -1) return true; // All cells filled

    int b = (bestR / 3) * 3 + (bestC / 3);
    for (int num = 1; num <= 9; num++) {
        int bit = 1 << num;
        if (bestMask & bit) {
            grid[bestR][bestC] = num;
            rowMask[bestR] |= bit;
            colMask[bestC] |= bit;
            boxMask[b] |= bit;

            if (solveSudokuRecursive(grid, rowMask, colMask, boxMask))
                return true;

            grid[bestR][bestC] = 0;
            rowMask[bestR] &= ~bit;
            colMask[bestC] &= ~bit;
            boxMask[b] &= ~bit;
        }
    }
    return false;
}

bool solveSudoku(Mat& board) {
    if (board.rows != 9 || board.cols != 9) return false;

    int grid[9][9];
    int rowMask[9] = {0}, colMask[9] = {0}, boxMask[9] = {0};

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            int val = board.at<uchar>(r, c);
            grid[r][c] = val;
            if (val >= 1 && val <= 9) {
                int bit = 1 << val;
                int b = (r / 3) * 3 + (c / 3);
                if ((rowMask[r] & bit) || (colMask[c] & bit) || (boxMask[b] & bit)) {
                    return false; // Duplicate conflict in initial board
                }
                rowMask[r] |= bit;
                colMask[c] |= bit;
                boxMask[b] |= bit;
            }
        }
    }

    if (!solveSudokuRecursive(grid, rowMask, colMask, boxMask))
        return false;

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            board.at<uchar>(r, c) = (uchar)grid[r][c];
        }
    }
    return true;
}

bool resolveAndSolveSudoku(Mat& board, const Mat& altBoard, const Mat& confidenceMat) {
    Mat candidate = board.clone();
    if (solveSudoku(candidate)) {
        board = candidate;
        return true;
    }

    // Collect filled cells sorted by ascending confidence margin
    struct CellCand {
        int r, c;
        float margin;
    };
    vector<CellCand> cells;
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (board.at<uchar>(r, c) != 0) {
                cells.push_back({r, c, confidenceMat.at<float>(r, c)});
            }
        }
    }
    std::sort(cells.begin(), cells.end(), [](const CellCand& a, const CellCand& b) {
        return a.margin < b.margin;
    });

    // Try single-cell substitution with runner-up KNN prediction
    for (size_t i = 0; i < cells.size(); i++) {
        int r = cells[i].r;
        int c = cells[i].c;
        uchar alt = altBoard.at<uchar>(r, c);
        if (alt != board.at<uchar>(r, c)) {
            candidate = board.clone();
            candidate.at<uchar>(r, c) = alt;
            if (solveSudoku(candidate)) {
                board.at<uchar>(r, c) = alt; // Fix initial OCR mistake as well
                board = candidate;
                return true;
            }
        }
    }

    // Try two-cell substitution on lowest-confidence cells
    size_t limit = std::min<size_t>(cells.size(), 10);
    for (size_t i = 0; i < limit; i++) {
        for (size_t j = i + 1; j < limit; j++) {
            candidate = board.clone();
            candidate.at<uchar>(cells[i].r, cells[i].c) = altBoard.at<uchar>(cells[i].r, cells[i].c);
            candidate.at<uchar>(cells[j].r, cells[j].c) = altBoard.at<uchar>(cells[j].r, cells[j].c);
            if (solveSudoku(candidate)) {
                board = candidate;
                return true;
            }
        }
    }

    return false;
}

void overlaySolvedNumbers(Mat& image, const Mat& originalBoard, const Mat& solvedBoard, int maxLength, const Mat& invTransform) {
    Mat overlayWarped = Mat::zeros(Size(maxLength, maxLength), CV_8UC3);
    Mat maskWarped = Mat::zeros(Size(maxLength, maxLength), CV_8UC1);

    double cellSize = (double)maxLength / 9.0;
    double fontScale = std::max(0.55, cellSize / 42.0);
    int thickness = std::max(2, (int)std::round(fontScale * 2.2));
    int outlineThickness = thickness + 2;
    int fontFace = FONT_HERSHEY_DUPLEX;

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            uchar origVal = originalBoard.at<uchar>(r, c);
            uchar solvedVal = solvedBoard.at<uchar>(r, c);
            if (origVal == 0 && solvedVal >= 1 && solvedVal <= 9) {
                string text = std::to_string((int)solvedVal);
                int baseline = 0;
                Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);

                int centerX = (int)std::round((c + 0.5) * cellSize);
                int centerY = (int)std::round((r + 0.5) * cellSize);

                Point textOrg(centerX - textSize.width / 2, centerY + textSize.height / 2);

                // E2i corporate colors: outline in Primary Dark (#244B5A), fill in Primary Mint (#99E6D8)
                putText(overlayWarped, text, textOrg, fontFace, fontScale, E2I_PRIMARY_DARK, outlineThickness + 1, LINE_AA);
                putText(overlayWarped, text, textOrg, fontFace, fontScale, E2I_PRIMARY_MINT, thickness, LINE_AA);

                putText(maskWarped, text, textOrg, fontFace, fontScale, Scalar(255), outlineThickness + 1, LINE_AA);
            }
        }
    }

    Mat warpedOverlay, warpedMask;
    warpPerspective(overlayWarped, warpedOverlay, invTransform, image.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
    warpPerspective(maskWarped, warpedMask, invTransform, image.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(0));

    // Smooth alpha blending using warpedMask
    for (int y = 0; y < image.rows; y++) {
        const uchar* mRow = warpedMask.ptr<uchar>(y);
        const Vec3b* oRow = warpedOverlay.ptr<Vec3b>(y);
        Vec3b* dstRow = image.ptr<Vec3b>(y);
        for (int x = 0; x < image.cols; x++) {
            uchar alpha = mRow[x];
            if (alpha > 15) {
                float a = alpha / 255.0f;
                dstRow[x][0] = (uchar)std::min(255.0f, dstRow[x][0] * (1.0f - a) + oRow[x][0] * a);
                dstRow[x][1] = (uchar)std::min(255.0f, dstRow[x][1] * (1.0f - a) + oRow[x][1] * a);
                dstRow[x][2] = (uchar)std::min(255.0f, dstRow[x][2] * (1.0f - a) + oRow[x][2] * a);
            }
        }
    }
}

Mat createSolvedBoardCrop(const Mat& colorWarpedBoard, const Mat& originalBoard, const Mat& solvedBoard) {
    Mat crop = colorWarpedBoard.clone();
    if (crop.empty()) return crop;
    if (crop.channels() == 1) {
        cvtColor(crop, crop, COLOR_GRAY2BGR);
    }

    int maxLength = crop.rows;
    double cellSize = (double)maxLength / 9.0;
    double fontScale = std::max(0.55, cellSize / 42.0);
    int thickness = std::max(2, (int)std::round(fontScale * 2.2));
    int outlineThickness = thickness + 2;
    int fontFace = FONT_HERSHEY_DUPLEX;

    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            uchar origVal = originalBoard.at<uchar>(r, c);
            uchar solvedVal = solvedBoard.at<uchar>(r, c);
            if (origVal == 0 && solvedVal >= 1 && solvedVal <= 9) {
                string text = std::to_string((int)solvedVal);
                int baseline = 0;
                Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);

                int centerX = (int)std::round((c + 0.5) * cellSize);
                int centerY = (int)std::round((r + 0.5) * cellSize);
                Point textOrg(centerX - textSize.width / 2, centerY + textSize.height / 2);

                // E2i corporate colors: outline in Primary Dark (#244B5A), fill in Primary Mint (#99E6D8)
                putText(crop, text, textOrg, fontFace, fontScale, E2I_PRIMARY_DARK, outlineThickness + 1, LINE_AA);
                putText(crop, text, textOrg, fontFace, fontScale, E2I_PRIMARY_MINT, thickness, LINE_AA);
            }
        }
    }
    return crop;
}

void drawSolvedCropPiP(Mat& frame, const Mat& solvedCrop, const Point2f liveQuad[4], int topOffset, bool drawCalloutLines) {
    if (frame.empty() || solvedCrop.empty()) return;

    int pipSize = std::max(120, std::min(240, (int)(std::min(frame.rows, frame.cols) * 0.34)));
    int rightMargin = 14;
    int pipX = frame.cols - pipSize - rightMargin;
    int pipY = std::max(14, topOffset);

    if (pipX < 0 || pipY + pipSize > frame.rows) return;
    Rect pipRect(pipX, pipY, pipSize, pipSize);

    Point2f pipCorners[4] = {
        Point2f((float)pipRect.x, (float)pipRect.y),
        Point2f((float)(pipRect.x + pipRect.width), (float)pipRect.y),
        Point2f((float)(pipRect.x + pipRect.width), (float)(pipRect.y + pipRect.height)),
        Point2f((float)pipRect.x, (float)(pipRect.y + pipRect.height))
    };

    // Draw semi-transparent projection / callout lines connecting the live recognized quad to the top-right PiP box
    if (drawCalloutLines && liveQuad != nullptr) {
        Mat lineOverlay = frame.clone();
        const int tangentCorners[2] = {0, 2};
        for (int idx : tangentCorners) {
            line(lineOverlay, liveQuad[idx], pipCorners[idx], E2I_PRIMARY_DARK, 4, LINE_AA);
            line(lineOverlay, liveQuad[idx], pipCorners[idx], E2I_PRIMARY_MINT, 2, LINE_AA);
        }
        for (int k = 0; k < 4; k++) {
            circle(lineOverlay, liveQuad[k], 6, E2I_PRIMARY_DARK, FILLED, LINE_AA);
            circle(lineOverlay, liveQuad[k], 4, E2I_PRIMARY_MINT, FILLED, LINE_AA);
        }
        addWeighted(lineOverlay, 0.80, frame, 0.20, 0.0, frame);
    }

    // Draw subtle drop shadow behind PiP box in E2i Secondary Slate (#353E47)
    Rect shadowRect(std::min(frame.cols - pipSize, pipRect.x + 5),
                    std::min(frame.rows - pipSize, pipRect.y + 5),
                    pipSize, pipSize);
    rectangle(frame, shadowRect, E2I_SEC_SLATE, FILLED);

    // Resize cropped solved Sudoku and copy into top-right PiP region (occluding lines behind the box)
    Mat pipThumb;
    resize(solvedCrop, pipThumb, Size(pipSize, pipSize), 0, 0, INTER_AREA);
    pipThumb.copyTo(frame(pipRect));

    // Draw crisp outer border and corner anchor points on the PiP box in E2i primary colors
    rectangle(frame, pipRect, E2I_PRIMARY_DARK, 4, LINE_AA);
    rectangle(frame, pipRect, E2I_PRIMARY_MINT, 2, LINE_AA);
    for (int k = 0; k < 4; k++) {
        circle(frame, pipCorners[k], 5, E2I_PRIMARY_DARK, FILLED, LINE_AA);
        circle(frame, pipCorners[k], 3, E2I_PRIMARY_MINT, FILLED, LINE_AA);
    }
}




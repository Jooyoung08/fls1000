import ROOT
from array import array

# Python for FLS-1000 Ex&Em txt file to ROOT file.

# Path
file_path = ""
data_path = ""
txt_file_name = ""
txt_file = file_path + txt_file_name + ".txt"
root_file = data_path + txt_file_name + ".root"

# ROOT 파일 생성
# spec : Wavelength & counts
# info : 기타정보
f = ROOT.TFile(root_file, "RECREATE")
tree = ROOT.TTree("spec", "FLS-1000")
itree = ROOT.TTree("info", "FLS-1000")

# 변수 정의
wavelength = array('f', [0.0])
counts = array('f', [0.0])

# start : Ex or Em start wavelength
# stop  : Ex or Em stop wavelength
# step  : Ex or Em step wavelength
# exem  : Ex or Em wavelength
start = array('f', [0.0])
stop = array('f', [0.0])
step = array('f', [0.0])
exem = array('f',[0.0])

tree.Branch("wavelength", wavelength, "wavelength/F")
tree.Branch("counts", counts, "counts/F")

itree.Branch("start", start, "start/F")
itree.Branch("stop", stop, "stop/F")
itree.Branch("step", step, "step/F")
itree.Branch("exem", exem, "exem/F")

# txt 파일 읽기
with open(txt_file) as infile:
    for line in infile:
        line = line.strip()
        indata = line.split(",")
        if indata[0] == "Start":
            start[0] = float(indata[1])
        if indata[0] == "Stop":
            stop[0] = float(indata[1])
        if indata[0] == "Step":
            step[0] = float(indata[1])
        if indata[0] == "Fixed/Offset":
            exem[0] = float(indata[1])
            itree.Fill()
        # print(indata)
        if not line or line[0].isalpha():   # 헤더(문자 시작) 스킵
            continue
        try:
            cols = line.split(",")
            if len(cols) >= 2:
                wavelength[0] = float(cols[0])
                counts[0] = float(cols[1])
                tree.Fill()
        except ValueError:
            continue  # 숫자 변환 불가능한 줄은 무시

# 저장
tree.Write()
itree.Write()
f.Close()
print("{} ROOT 파일 저장 완료".format(txt_file_name))


import sys
from cyvcf2 import VCF

if len(sys.argv) != 2:
	print(f"Usage: {sys.argv[0]} <in.vcf>")
	sys.exit(1)

vcf_file = sys.argv[1]
for variant in VCF(vcf_file):
	if variant.FILTER == None:  # when FILTER == "PASS", cyvcf2 puts None on the FILTER variable
		both_genotypes = variant.genotypes[0][:2]
		gt_1 = both_genotypes[0]
		gt_2 = both_genotypes[1]
		if gt_1 != -1 and gt_2 != -1:
			print(str(gt_1) if gt_1==gt_2 else f"{gt_1}/{gt_2}")

// -*- C++ -*-
//
// Package:    BToKstarMuMu
// Class:      BToKstarMuMu
// 
/**\class BToKstarMuMu BToKstarMuMu.cc BphAna/BToKstarMuMu/src/BToKstarMuMu.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Xin Shi <Xin.Shi@cern.ch>
//         Created:  Mon Jan 28 11:06:56 CET 2013
// $Id$
//
//



// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/GenericParticle.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/RefCountedKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"

#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"

#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TH1.h>

using namespace std;

// 
// Constans
// 

const int MUONMINUS_PDG_ID = 13; 
const int PIONPLUS_PDG_ID = 211; 
const int KSHORTZERO_PDG_ID = 310; 
const int KSTARPLUS_PDG_ID = 323; 
const int BPLUS_PDG_ID = 521; 


// Structures 
struct HistArgs{
  char name[128];
  char title[128];
  int n_bins;
  double x_min;
  double x_max;
};

enum HistName{
  h_mumumass,
  h_kshortmass,
  kHistNameSize
};

// Global hist args

HistArgs hist_args[kHistNameSize] = {
  // name, title, n_bins, x_min, x_max  
  {"h_mumumass", "#mu^{+}#mu^{-} invariant mass; M(#mu^{+}#mu^{-}) [GeV]", 100, 2, 4},
  {"h_kshortmass", "Kshort mass; M(Kshort) [GeV]", 100, 0.2, 0.8},
};

// Define histograms 
TH1F *BToKstarMuMuFigures[kHistNameSize];

//
// class declaration
//

class BToKstarMuMu : public edm::EDAnalyzer {
public:
  explicit BToKstarMuMu(const edm::ParameterSet&);
  ~BToKstarMuMu();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& );
  
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  

  // bool buildBuToPiMuMu(const edm::Event &); 
  bool buildBuToKstarMuMu(const edm::Event &); 

  void computeLS (double, double, double, double, double, double, double, double, double, 
		  double, double, double, double, double, double, double, double, double, 
		  double* deltaD, double* deltaDErr); 

  void computeCosAlpha (double Vx, double Vy, double Vz, double Wx, double Wy, double Wz,
			double VxErr2, double VyErr2, double VzErr2, double VxyCov,
			double VxzCov, double VyzCov, double WxErr2, double WyErr2,
			double WzErr2, double WxyCov, double WxzCov, double WyzCov,
			double* cosAlpha, double* cosAlphaErr); 
  
  void computeCtau(RefCountedKinematicTree, double &, double &);
  void clearVariables(); 


  bool hasBeamSpot(const edm::Event&);

  bool hasDimuon(const edm::Event &); 

  bool hasGoodClosestApproachTracks (const reco::TransientTrack,
				     const reco::TransientTrack, double &);

  bool hasGoodKshortVertex(const vector<reco::TrackRef>, RefCountedKinematicTree &); 
  bool hasGoodKshortVertexMKC(const vector<reco::TrackRef>, RefCountedKinematicTree &); 
  bool hasGoodDimuonKshortMass(const edm::Event&);  
  bool hasGoodTrackDcaBs (const reco::TransientTrack, double &, double &); 
  bool hasGoodTrackDcaPoint (const reco::TransientTrack, const GlobalPoint, 
			     double, double &, double &);

  bool hasGoodKstarChargedVertex(vector<reco::TrackRef>, const reco::TrackRef, 
				 RefCountedKinematicTree&); 
  bool hasGoodKstarChargedMass(RefCountedKinematicTree); 

  bool hasGoodBuMass(RefCountedKinematicTree); 
  bool hasGoodBu3Mass(RefCountedKinematicTree); 
  bool hasGoodBuVertex(reco::TrackRef, reco::TrackRef, 
		       reco::TrackRef, RefCountedKinematicTree&); 
  bool hasGoodBuVertex(const pat::CompositeCandidate, const pat::CompositeCandidate,
		       RefCountedKinematicTree&, RefCountedKinematicTree &); 
  bool hasGoodBuVertex(const pat::CompositeCandidate, const pat::CompositeCandidate,
		       RefCountedKinematicTree&); 
  bool hasGoodBuVertex(const pat::CompositeCandidate,  const pat::CompositeCandidate,
		       const pat::GenericParticle, RefCountedKinematicTree&); 

  bool hasGoodMuMuVertex (const reco::TransientTrack, const reco::TransientTrack,
			  reco::TransientTrack &, reco::TransientTrack &, 
			  double &, double &, double &, double &, double &);

  bool hasGoodPionTrack(const edm::Event&, const pat::GenericParticle);

  bool hasKshort(const edm::Event &); 
  bool hasKstarCharged(const edm::Event &); 

  bool hasPrimaryVertex(const edm::Event &); 

  void hltReport(const edm::Event&);

  bool isGenKstarCharged(const reco::Candidate *);
  bool isGenKshort(const reco::Candidate *);
  bool isGenMuonP(const reco::Candidate *);

  bool matchMuonTrack (const edm::Event&, const reco::TrackRef);
  bool matchMuonTracks (const edm::Event&, const vector<reco::TrackRef>);
  bool matchKshortTrack (const edm::Event&, const reco::TrackRef);
  bool matchPrimaryVertexTracks (); 

  void saveBuToKstarMuMu(RefCountedKinematicTree); 
  void saveBuVertex(RefCountedKinematicTree); 
  void saveBuCosAlpha(RefCountedKinematicTree); 
  void saveBuLsig(RefCountedKinematicTree);
  void saveBuCtau(RefCountedKinematicTree); 
  // void saveBuToPiMuMu(RefCountedKinematicTree); 
  // void saveBu3Vertex(RefCountedKinematicTree); 
  // void saveBu3CosAlpha(RefCountedKinematicTree); 
  // bool saveBu3Lsig(RefCountedKinematicTree);
  // void saveBu3Ctau(RefCountedKinematicTree); 
  void saveGenInfo(const edm::Event&); 
  void saveKshortVariables(reco::VertexCompositeCandidate); 

  void saveSoftMuonVariables(pat::Muon, pat::Muon, reco::TrackRef, reco::TrackRef); 
  void saveDimuVariables(double, double, double, double, double, double, double, double, 
			 double, double, reco::TransientTrack, reco::TransientTrack);
  void saveMuonTriggerMatches(const pat::Muon, const pat::Muon); 

  // ----------member data ---------------------------

  // --- input from python file --- 
  string FileName_; 
  bool SaveGenInfo_; 
  edm::InputTag GenParticlesLabel_;
  edm::InputTag TriggerResultsLabel_;
  vector<string> TriggerNames_, LastFilterNames_;
  map<string, string> mapTriggerToLastFilter_;
  edm::InputTag BeamSpotLabel_;
  edm::InputTag VertexLabel_;

  // Muon 
  edm::InputTag MuonLabel_;
  double MuonMinPt_; 
  double MuonMaxEta_; 
  double TrkMaxDcaBs_; 
  double TrkMaxR_;
  double TrkMaxZ_; 
  double MuMuMaxDca_; 
  double MuMuMinVtxCl_; 
  double MuMuMinPt_; 
  double MuMuMinInvMass_; 
  double MuMuMaxInvMass_; 
  double MuMuMinLxySigmaBs_; 
  double MuMuMinCosAlphaBs_; 
  ParticleMass MuonMass_; 
  float MuonMassErr_; 
  pat::CompositeCandidateCollection DimuonCandidates_; 

  // Kshort 
  edm::InputTag KshortLabel_;
  edm::InputTag TrackLabel_;
  ParticleMass PionMass_; 
  float PionMassErr_; 
  ParticleMass KshortMass_; 
  float KshortMassErr_; 
  pat::CompositeCandidateCollection KshortCandidates_; 

  double DimukshortMinMass_, DimukshortMaxMass_; 
  
  // Kstar
  double KstarChargedTrackMinPt_; 
  double KstarMinMass_, KstarMaxMass_; 
  pat::CompositeCandidateCollection KstarChargedCandidates_; 

  // B meson
  pat::CompositeCandidateCollection BuCandidates_;  
  //   BuToPiMuMuCandidates_; 
  double BMaxMass_; 
  double BuMass_; 
  int BMaxCandNum_;  
  // double B3MinMass_, B3MaxMass_, B3MinLsBs_; 
  

  // Across the event 
  reco::BeamSpot beamSpot_;  
  edm::ESHandle<MagneticField> bFieldHandle_;
  reco::Vertex primaryVertex_;


  // ---- Root Variables ---- 
  TFile* fout_;
  TTree* tree_;
  
  unsigned int run, event, lumiblock, nprivtx; 
  vector<string> *triggernames;
  vector<int> *triggerprescales;
  
  // dimu
  vector<double> *mumdcabs, *mumdcabserr, *mumpx, *mumpy, *mumpz; 
  vector<double> *mupdcabs, *mupdcabserr, *muppx, *muppy, *muppz; 
  vector<double> *mumudca, *mumuvtxcl, *mumulsbs, *mumulsbserr, 
    *mumucosalphabs, *mumucosalphabserr; 
  vector<bool> *mumisgoodmuon, *mupisgoodmuon ; 
  vector<int> *mumnpixhits, *mupnpixhits, *mumnpixlayers, *mupnpixlayers; 
  vector<int> *mumntrkhits, *mupntrkhits, *mumntrklayers, *mupntrklayers; 
  vector<double> *mumnormchi2, *mupnormchi2; 
  vector<double> *mumdxyvtx, *mupdxyvtx, *mumdzvtx, *mupdzvtx; 
  vector<string> *mumtriglastfilter, *muptriglastfilter; 

  // kshort 
  vector<double> *pimpx, *pimpy, *pimpz, *pimmass, *pimd0, *pimd0err; 
  vector<double> *pippx, *pippy, *pippz, *pipmass, *pipd0, *pipd0err; 
  vector<double> *kspx, *kspy, *kspz, *ksmass; 
  vector<double> *ksvtxx, *ksvtxy, *ksvtxz, *ksvtxcl; 
  
  // B to K* mu mu variables 
  vector<int> *bchg; // +1 for b+, -1 for b-
  vector<double> *bpx, *bpxerr, *bpy, *bpyerr, *bpz, *bpzerr, *bmass, *bmasserr; 
  vector<double> *bvtxcl, *bvtxx, *bvtxxerr, *bvtxy, *bvtxyerr, *bvtxz, *bvtxzerr; 
  vector<double> *bcosalphabs, *bcosalphabserr, *blsbs, *blsbserr, *bctau, *bctauerr; 
  
  vector<int> *bmu1chg, *bmu2chg, *bpi1chg; // +1 for mu+, -1 for mu-
  vector<double> *bmu1px, *bmu1py, *bmu1pz, *bmu1mass, *bmu1masserr,
    *bmu2px, *bmu2py, *bmu2pz, *bmu2mass, *bmu2masserr,
    *bpi1px, *bpi1py, *bpi1pz, *bpi1mass, *bpi1masserr,
    *bkspx, *bkspy, *bkspz, *bksmass, *bksmasserr; 


  // B to pi mu mu variables 
  // vector<int> *b3mu1chg, *b3mu2chg, *b3pi1chg; 
  // vector<double> *b3mu1px, *b3mu1py, *b3mu1pz, *b3mu2px, *b3mu2py, *b3mu2pz, 
  //   *b3pi1px, *b3pi1py, *b3pi1pz; 

  // vector<int> *b3chg; // +1 for b+, -1 for b-
  // vector<double> *b3px, *b3pxerr, *b3py, *b3pyerr, *b3pz, *b3pzerr, *b3mass, *b3masserr; 
  // vector<double> *b3vtxcl, *b3vtxx, *b3vtxxerr, *b3vtxy, *b3vtxyerr, *b3vtxz, *b3vtxzerr; 
  // vector<double> *b3cosalphabs, *b3cosalphabserr, *b3lsbs, *b3lsbserr, *b3ctau, *b3ctauerr; 

  // GenInfo
  // vector<int> *genbchg; // +1 for b+, -1 for b-
  vector<double> *genbpx, *genbpy, *genbpz;
  vector<double> *genkstpx, *genkstpy, *genkstpz;
  vector<double> *genkspx, *genkspy, *genkspz;
  vector<double> *genpipx, *genpipy, *genpipz;
  vector<double> *genmumpx, *genmumpy, *genmumpz;
  vector<double> *genmuppx, *genmuppy, *genmuppz;
  
 
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
BToKstarMuMu::BToKstarMuMu(const edm::ParameterSet& iConfig):
  FileName_(iConfig.getParameter<string>("FileName")),
  SaveGenInfo_(iConfig.getUntrackedParameter<bool>("SaveGenInfo")),
  GenParticlesLabel_(iConfig.getParameter<edm::InputTag>("GenParticlesLabel")),
  TriggerResultsLabel_(iConfig.getParameter<edm::InputTag>("TriggerResultsLabel")),
  TriggerNames_(iConfig.getParameter< vector<string> >("TriggerNames")),
  LastFilterNames_(iConfig.getParameter< vector<string> >("LastFilterNames")),
  BeamSpotLabel_(iConfig.getParameter<edm::InputTag>("BeamSpotLabel")),
  VertexLabel_(iConfig.getParameter<edm::InputTag>("VertexLabel")),
  MuonLabel_(iConfig.getParameter<edm::InputTag>("MuonLabel")),
  MuonMinPt_(iConfig.getUntrackedParameter<double>("MuonMinPt")),
  MuonMaxEta_(iConfig.getUntrackedParameter<double>("MuonMaxEta")),
  TrkMaxDcaBs_(iConfig.getUntrackedParameter<double>("TrkMaxDcaBs")),
  TrkMaxR_(iConfig.getUntrackedParameter<double>("TrkMaxR")),
  TrkMaxZ_(iConfig.getUntrackedParameter<double>("TrkMaxZ")),
  MuMuMaxDca_(iConfig.getUntrackedParameter<double>("MuMuMaxDca")),
  MuMuMinVtxCl_(iConfig.getUntrackedParameter<double>("MuMuMinVtxCl")),
  MuMuMinPt_(iConfig.getUntrackedParameter<double>("MuMuMinPt")),
  MuMuMinInvMass_(iConfig.getUntrackedParameter<double>("MuMuMinInvMass")),
  MuMuMaxInvMass_(iConfig.getUntrackedParameter<double>("MuMuMaxInvMass")),
  MuMuMinLxySigmaBs_(iConfig.getUntrackedParameter<double>("MuMuMinLxySigmaBs")), 
  MuMuMinCosAlphaBs_(iConfig.getUntrackedParameter<double>("MuMuMinCosAlphaBs")), 
  MuonMass_(iConfig.getUntrackedParameter<double>("MuonMass")),
  MuonMassErr_(iConfig.getUntrackedParameter<double>("MuonMassErr")),
  KshortLabel_(iConfig.getParameter<edm::InputTag>("KshortLabel")),
  TrackLabel_(iConfig.getParameter<edm::InputTag>("TrackLabel")),
  PionMass_(iConfig.getUntrackedParameter<double>("PionMass")),
  PionMassErr_(iConfig.getUntrackedParameter<double>("PionMassErr")),
  KshortMass_(iConfig.getUntrackedParameter<double>("KshortMass")),
  KshortMassErr_(iConfig.getUntrackedParameter<double>("KshortMassErr")),
  DimukshortMinMass_(iConfig.getUntrackedParameter<double>("DimukshortMinMass")),
  DimukshortMaxMass_(iConfig.getUntrackedParameter<double>("DimukshortMaxMass")),

  KstarChargedTrackMinPt_(iConfig.getUntrackedParameter<double>("KstarChargedTrackMinPt")),
  KstarMinMass_(iConfig.getUntrackedParameter<double>("KstarMinMass")),
  KstarMaxMass_(iConfig.getUntrackedParameter<double>("KstarMaxMass")),
  BMaxMass_(iConfig.getUntrackedParameter<double>("BMaxMass")),
  BuMass_(iConfig.getUntrackedParameter<double>("BuMass")),
  BMaxCandNum_(iConfig.getUntrackedParameter<int>("BMaxCandNum")),
  // B3MinMass_(iConfig.getUntrackedParameter<double>("B3MinMass")),
  // B3MaxMass_(iConfig.getUntrackedParameter<double>("B3MaxMass")),
  // B3MinLsBs_(iConfig.getUntrackedParameter<double>("B3MinLsBs")),


  tree_(0), 
  triggernames(0), triggerprescales(0), 
  mumdcabs(0), mumdcabserr(0), mumpx(0), mumpy(0), mumpz(0),
  mupdcabs(0),  mupdcabserr(0), muppx(0),  muppy(0),  muppz(0),
  mumudca(0),  mumuvtxcl(0),  mumulsbs(0),  mumulsbserr(0), 
  mumucosalphabs(0),  mumucosalphabserr(0), 
  mumisgoodmuon(0), mupisgoodmuon(0), 
  mumnpixhits(0), mupnpixhits(0), mumnpixlayers(0), mupnpixlayers(0), 
  mumntrkhits(0), mupntrkhits(0), mumntrklayers(0), mupntrklayers(0), 
  mumnormchi2(0), mupnormchi2(0), mumdxyvtx(0), mupdxyvtx(0), mumdzvtx(0), mupdzvtx(0), 
  mumtriglastfilter(0), muptriglastfilter(0), 

  pimpx(0), pimpy(0), pimpz(0), pimmass(0), pimd0(0), pimd0err(0), 
  pippx(0), pippy(0), pippz(0), pipmass(0), pipd0(0), pipd0err(0), 
  kspx(0), kspy(0), kspz(0), ksmass(0),
  ksvtxx(0), ksvtxy(0), ksvtxz(0), ksvtxcl(0), 
  bchg(0), bpx(0), bpxerr(0), bpy(0), bpyerr(0), bpz(0), bpzerr(0), bmass(0), bmasserr(0), 
  bvtxcl(0), bvtxx(0), bvtxxerr(0), bvtxy(0), bvtxyerr(0), bvtxz(0), bvtxzerr(0), 
  bcosalphabs(0), bcosalphabserr(0), blsbs(0), blsbserr(0), bctau(0), bctauerr(0), 

  bmu1chg(0), bmu2chg(0), bpi1chg(0), bmu1px(0),  bmu1py(0), bmu1pz(0), bmu1mass(0), bmu1masserr(0), 
  bmu2px(0),  bmu2py(0),  bmu2pz(0), bmu2mass(0), bmu2masserr(0),
  bpi1px(0),  bpi1py(0),  bpi1pz(0), bpi1mass(0), bpi1masserr(0), 
  bkspx(0),  bkspy(0),  bkspz(0), bksmass(0), bksmasserr(0), 

  // b3mu1chg(0), b3mu2chg(0), b3pi1chg(0),
  // b3mu1px(0),  b3mu1py(0),  b3mu1pz(0), 
  // b3mu2px(0),  b3mu2py(0),  b3mu2pz(0), 
  // b3pi1px(0),  b3pi1py(0),  b3pi1pz(0), 
  // b3chg(0), b3px(0), b3pxerr(0), b3py(0), b3pyerr(0), b3pz(0), b3pzerr(0), b3mass(0), b3masserr(0),
  // b3vtxcl(0), b3vtxx(0), b3vtxxerr(0), b3vtxy(0), b3vtxyerr(0), b3vtxz(0), b3vtxzerr(0), 
  // b3cosalphabs(0), b3cosalphabserr(0), b3lsbs(0), b3lsbserr(0), b3ctau(0), b3ctauerr(0), 

  genbpx(0), genbpy(0), genbpz(0), 
  genkstpx(0), genkstpy(0), genkstpz(0), 
  genkspx(0), genkspy(0), genkspz(0), 
  genpipx(0), genpipy(0), genpipz(0), 
  genmumpx(0), genmumpy(0), genmumpz(0),
  genmuppx(0), genmuppy(0), genmuppz(0)

{ 
  //now do what ever initialization is needed
  
  assert(TriggerNames_.size() == LastFilterNames_.size());
  for (size_t i = 0; i < TriggerNames_.size(); ++i)
    mapTriggerToLastFilter_[TriggerNames_[i]] = LastFilterNames_[i];
  
}


BToKstarMuMu::~BToKstarMuMu()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
BToKstarMuMu::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  clearVariables(); 

  run = iEvent.id().run() ;
  event = iEvent.id().event() ;
  lumiblock = iEvent.luminosityBlock(); 

  hltReport(iEvent);

  if ( hasBeamSpot(iEvent) ) {
    iSetup.get<IdealMagneticFieldRecord>().get(bFieldHandle_);      
    
    if ( bFieldHandle_.isValid() 
	 && hasPrimaryVertex(iEvent) 
	 && hasGoodDimuonKshortMass(iEvent) 
	 && hasDimuon(iEvent)
	 && hasKshort(iEvent) 
	 // && hasKstarCharged(iEvent) 
	 && buildBuToKstarMuMu(iEvent) )

      if (SaveGenInfo_) saveGenInfo(iEvent);
      tree_->Fill();
  }
  
  clearVariables(); 
}


// ------------ method called once each job just before starting event loop  ------------
void 
BToKstarMuMu::beginJob()
{
  fout_ = new TFile(FileName_.c_str(), "RECREATE");
  fout_->cd();

  for(int i=0; i<kHistNameSize; i++) {
    BToKstarMuMuFigures[i] = new TH1F(hist_args[i].name, hist_args[i].title,
				       hist_args[i].n_bins,
				       hist_args[i].x_min, hist_args[i].x_max);
  }
  

  tree_ = new TTree ("tree", "BToKstarMuMu");

  tree_->Branch("run", &run, "run/i");
  tree_->Branch("event", &event, "event/i");
  tree_->Branch("lumiblock", &lumiblock, "lumiblock/i");
  tree_->Branch("nprivtx", &nprivtx, "nprivtx/i");
  tree_->Branch("triggernames", &triggernames);
  tree_->Branch("triggerprescales", &triggerprescales);

  tree_->Branch("mumdcabs", &mumdcabs);
  tree_->Branch("mumdcabserr", &mumdcabserr);
  tree_->Branch("mumpx", &mumpx);
  tree_->Branch("mumpy", &mumpy);
  tree_->Branch("mumpz", &mumpz);
  tree_->Branch("mupdcabs", &mupdcabs);
  tree_->Branch("mupdcabserr", &mupdcabserr);
  tree_->Branch("muppx", &muppx);
  tree_->Branch("muppy", &muppy);
  tree_->Branch("muppz", &muppz);
  tree_->Branch("mumudca", &mumudca);
  tree_->Branch("mumuvtxcl", &mumuvtxcl);
  tree_->Branch("mumulsbs", &mumulsbs);
  tree_->Branch("mumulsbserr", &mumulsbserr);
  tree_->Branch("mumucosalphabs", &mumucosalphabs);
  tree_->Branch("mumucosalphabserr", &mumucosalphabserr);
  tree_->Branch("mumisgoodmuon", &mumisgoodmuon);
  tree_->Branch("mupisgoodmuon", &mupisgoodmuon);
  tree_->Branch("mumnpixhits", &mumnpixhits);
  tree_->Branch("mupnpixhits", &mupnpixhits);
  tree_->Branch("mumnpixlayers", &mumnpixlayers);
  tree_->Branch("mupnpixlayers", &mupnpixlayers);
  tree_->Branch("mumntrkhits", &mumntrkhits);
  tree_->Branch("mupntrkhits", &mupntrkhits);
  tree_->Branch("mumntrklayers", &mumntrklayers);
  tree_->Branch("mupntrklayers", &mupntrklayers);
  tree_->Branch("mumnormchi2", &mumnormchi2);
  tree_->Branch("mupnormchi2", &mupnormchi2);
  tree_->Branch("mumdxyvtx", &mumdxyvtx);
  tree_->Branch("mupdxyvtx", &mupdxyvtx);
  tree_->Branch("mumdzvtx", &mumdzvtx);
  tree_->Branch("mupdzvtx", &mupdzvtx);
  tree_->Branch("mumtriglastfilter", &mumtriglastfilter);
  tree_->Branch("muptriglastfilter", &muptriglastfilter);

  tree_->Branch("pimpx", &pimpx);
  tree_->Branch("pimpy", &pimpy);
  tree_->Branch("pimpz", &pimpz);
  tree_->Branch("pimmass", &pimmass);
  tree_->Branch("pimd0", &pimd0);
  tree_->Branch("pimd0err", &pimd0err);
  tree_->Branch("pippx", &pippx);
  tree_->Branch("pippy", &pippy);
  tree_->Branch("pippz", &pippz);
  tree_->Branch("pipmass", &pipmass);
  tree_->Branch("pipd0", &pipd0);
  tree_->Branch("pipd0err", &pipd0err);
  tree_->Branch("kspx", &kspx);
  tree_->Branch("kspy", &kspy);
  tree_->Branch("kspz", &kspz);
  tree_->Branch("ksmass", &ksmass);
  tree_->Branch("ksvtxx", &ksvtxx);
  tree_->Branch("ksvtxy", &ksvtxy);
  tree_->Branch("ksvtxz", &ksvtxz);
  tree_->Branch("ksvtxcl", &ksvtxcl);
 
  tree_->Branch("bchg", &bchg);
  tree_->Branch("bpx", &bpx);
  tree_->Branch("bpxerr", &bpxerr);
  tree_->Branch("bpy", &bpy);
  tree_->Branch("bpyerr", &bpyerr);
  tree_->Branch("bpz", &bpz);
  tree_->Branch("bpzerr", &bpzerr);
  tree_->Branch("bmass", &bmass);
  tree_->Branch("bmasserr", &bmasserr);
  tree_->Branch("bvtxcl", &bvtxcl);
  tree_->Branch("bvtxx", &bvtxx);
  tree_->Branch("bvtxxerr", &bvtxxerr);
  tree_->Branch("bvtxy", &bvtxy);
  tree_->Branch("bvtxyerr", &bvtxyerr);
  tree_->Branch("bvtxz", &bvtxz);
  tree_->Branch("bvtxzerr", &bvtxzerr);
  tree_->Branch("bcosalphabs", &bcosalphabs);
  tree_->Branch("bcosalphabserr", &bcosalphabserr);
  tree_->Branch("blsbs", &blsbs);
  tree_->Branch("blsbserr", &blsbserr);
  tree_->Branch("bctau", &bctau);
  tree_->Branch("bctauerr", &bctauerr);

  tree_->Branch("bmu1chg", &bmu1chg);
  tree_->Branch("bmu2chg", &bmu2chg);
  tree_->Branch("bpi1chg", &bpi1chg);
  tree_->Branch("bmu1px", &bmu1px);
  tree_->Branch("bmu1py", &bmu1py);
  tree_->Branch("bmu1pz", &bmu1pz);
  tree_->Branch("bmu1mass", &bmu1mass);
  tree_->Branch("bmu1masserr", &bmu1masserr);
  tree_->Branch("bmu2px", &bmu2px);
  tree_->Branch("bmu2py", &bmu2py);
  tree_->Branch("bmu2pz", &bmu2pz);
  tree_->Branch("bmu2mass", &bmu2mass);
  tree_->Branch("bmu2masserr", &bmu2masserr);
  tree_->Branch("bpi1px", &bpi1px);
  tree_->Branch("bpi1py", &bpi1py);
  tree_->Branch("bpi1pz", &bpi1pz);
  tree_->Branch("bpi1mass", &bpi1mass);
  tree_->Branch("bpi1masserr", &bpi1masserr);
  tree_->Branch("bkspx", &bkspx);
  tree_->Branch("bkspy", &bkspy);
  tree_->Branch("bkspz", &bkspz);
  tree_->Branch("bksmass", &bksmass);
  tree_->Branch("bksmasserr", &bksmasserr);

  // B to pi mu mu 
  // tree_->Branch("b3mu1chg", &b3mu1chg);
  // tree_->Branch("b3mu2chg", &b3mu2chg);
  // tree_->Branch("b3pi1chg", &b3pi1chg);
  // tree_->Branch("b3mu1px", &b3mu1px);
  // tree_->Branch("b3mu1py", &b3mu1py);
  // tree_->Branch("b3mu1pz", &b3mu1pz);
  // tree_->Branch("b3mu2px", &b3mu2px);
  // tree_->Branch("b3mu2py", &b3mu2py);
  // tree_->Branch("b3mu2pz", &b3mu2pz);
  // tree_->Branch("b3pi1px", &b3pi1px);
  // tree_->Branch("b3pi1py", &b3pi1py);
  // tree_->Branch("b3pi1pz", &b3pi1pz);
  // tree_->Branch("b3chg", &b3chg);
  // tree_->Branch("b3px", &b3px);
  // tree_->Branch("b3pxerr", &b3pxerr);
  // tree_->Branch("b3py", &b3py);
  // tree_->Branch("b3pyerr", &b3pyerr);
  // tree_->Branch("b3pz", &b3pz);
  // tree_->Branch("b3pzerr", &b3pzerr);
  // tree_->Branch("b3mass", &b3mass);
  // tree_->Branch("b3masserr", &b3masserr);
  // tree_->Branch("b3vtxcl", &b3vtxcl);
  // tree_->Branch("b3vtxx", &b3vtxx);
  // tree_->Branch("b3vtxxerr", &b3vtxxerr);
  // tree_->Branch("b3vtxy", &b3vtxy);
  // tree_->Branch("b3vtxyerr", &b3vtxyerr);
  // tree_->Branch("b3vtxz", &b3vtxz);
  // tree_->Branch("b3vtxzerr", &b3vtxzerr);
  // tree_->Branch("b3cosalphabs", &b3cosalphabs);
  // tree_->Branch("b3cosalphabserr", &b3cosalphabserr);
  // tree_->Branch("b3lsbs", &b3lsbs);
  // tree_->Branch("b3lsbserr", &b3lsbserr);
  // tree_->Branch("b3ctau", &b3ctau);
  // tree_->Branch("b3ctauerr", &b3ctauerr);

  if (SaveGenInfo_) {
    tree_->Branch("genbpx", &genbpx);
    tree_->Branch("genbpy", &genbpy);
    tree_->Branch("genbpz", &genbpz);
    tree_->Branch("genkstpx", &genkstpx);
    tree_->Branch("genkstpy", &genkstpy);
    tree_->Branch("genkstpz", &genkstpz);
    tree_->Branch("genkspx", &genkspx);
    tree_->Branch("genkspy", &genkspy);
    tree_->Branch("genkspz", &genkspz);
    tree_->Branch("genpipx", &genpipx);
    tree_->Branch("genpipy", &genpipy);
    tree_->Branch("genpipz", &genpipz);
    tree_->Branch("genmumpx", &genmumpx);
    tree_->Branch("genmumpy", &genmumpy);
    tree_->Branch("genmumpz", &genmumpz);
    tree_->Branch("genmuppx", &genmuppx);
    tree_->Branch("genmuppy", &genmuppy);
    tree_->Branch("genmuppz", &genmuppz);
  }


}

// ------------ method called once each job just after ending the event loop  ------------
void 
BToKstarMuMu::endJob() 
{
  fout_->cd();
  tree_->Write();
  
  for(int i = 0; i < kHistNameSize; i++) {
    BToKstarMuMuFigures[i]->Write();
    BToKstarMuMuFigures[i]->Delete();
  }
  fout_->Close();
}

// ------------ method called when starting to processes a run  ------------
void 
BToKstarMuMu::beginRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
BToKstarMuMu::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
BToKstarMuMu::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
BToKstarMuMu::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
BToKstarMuMu::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



void 
BToKstarMuMu::clearVariables(){
  run = 0; 
  event = 0;
  lumiblock = 0;
  nprivtx = 0; 
  triggernames->clear();
  triggerprescales->clear();
  mumdcabs->clear();  mumdcabserr->clear();  mumpx->clear();   mumpy->clear();   mumpz->clear(); 
  mupdcabs->clear();  mupdcabserr->clear();  muppx->clear();   muppy->clear();   muppz->clear(); 
  mumudca->clear();  mumuvtxcl->clear();   mumulsbs->clear();  mumulsbserr->clear(); 
  mumucosalphabs->clear();  mumucosalphabserr->clear();  
  mumisgoodmuon->clear();  mupisgoodmuon->clear(); 
  mumnpixhits->clear();  mupnpixhits->clear();  mumnpixlayers->clear();  mupnpixlayers->clear(); 
  mumntrkhits->clear();  mupntrkhits->clear();  mumntrklayers->clear();  mupntrklayers->clear(); 
  mumnormchi2->clear(); mupnormchi2->clear(); mumdxyvtx->clear(); mupdxyvtx->clear(); 
  mumdzvtx->clear(); mupdzvtx->clear();  mumtriglastfilter->clear(); muptriglastfilter->clear(); 
  pimpx->clear(); pimpy->clear(); pimpz->clear(); pimmass->clear(); pimd0->clear(); pimd0err->clear();
  pippx->clear(); pippy->clear(); pippz->clear(); pipmass->clear(); pipd0->clear(); pipd0err->clear();
  kspx->clear(); kspy->clear(); kspz->clear(); ksmass->clear();
  ksvtxx->clear(); ksvtxy->clear(); ksvtxz->clear(); ksvtxcl->clear();
  DimuonCandidates_.clear();  KshortCandidates_.clear(); 
  KstarChargedCandidates_.clear(); BuCandidates_.clear();
  // BuToPiMuMuCandidates_.clear();
  bpx->clear(); bpxerr->clear(); bpy->clear();  bpyerr->clear(); bpz->clear(); bpzerr->clear(); 
  bchg->clear(); bmass->clear(); bmasserr->clear(); 
  bvtxcl->clear(); bvtxx->clear(); bvtxxerr->clear(); bvtxy->clear(); bvtxyerr->clear();
  bvtxz->clear(); bvtxzerr->clear(); bcosalphabs->clear(); bcosalphabserr->clear(); 
  blsbs->clear(); blsbserr->clear();  bctau->clear(); bctauerr->clear(); 

  bmu1chg->clear(); bmu2chg->clear(); bpi1chg->clear(); 
  bmu1px->clear();   bmu1py->clear();   bmu1pz->clear(); bmu1mass->clear();  bmu1masserr->clear();
  bmu2px->clear();   bmu2py->clear();   bmu2pz->clear(); bmu2mass->clear();  bmu2masserr->clear();
  bpi1px->clear();   bpi1py->clear();   bpi1pz->clear(); bpi1mass->clear();  bpi1masserr->clear();
  bkspx->clear();   bkspy->clear();   bkspz->clear(); bksmass->clear();  bksmasserr->clear();

  // b3mu1chg->clear(); b3mu2chg->clear(); b3pi1chg->clear(); 
  // b3mu1px->clear();   b3mu1py->clear();   b3mu1pz->clear(); 
  // b3mu2px->clear();   b3mu2py->clear();   b3mu2pz->clear(); 
  // b3pi1px->clear();   b3pi1py->clear();   b3pi1pz->clear();
  // b3chg->clear();  
  // b3px->clear(); b3pxerr->clear(); b3py->clear();  b3pyerr->clear(); b3pz->clear(); b3pzerr->clear(); 
  // b3chg->clear(); b3mass->clear(); b3masserr->clear();  b3cosalphabs->clear(); b3cosalphabserr->clear(); 
  // b3lsbs->clear(); b3lsbserr->clear();  b3ctau->clear(); b3ctauerr->clear(); 
  if (SaveGenInfo_) {
    genbpx->clear();    genbpy->clear();    genbpz->clear(); 
    genkstpx->clear();  genkstpy->clear();  genkstpz->clear(); 
    genkspx->clear();   genkspy->clear();   genkspz->clear(); 
    genpipx->clear();   genpipy->clear();   genpipz->clear(); 
    genmumpx->clear();  genmumpy->clear();  genmumpz->clear(); 
    genmuppx->clear();  genmuppy->clear();  genmuppz->clear(); 
  }

}


void
BToKstarMuMu::hltReport(const edm::Event& iEvent)
{
  edm::Handle<edm::TriggerResults> hltTriggerResults;
  try {iEvent.getByLabel( TriggerResultsLabel_, hltTriggerResults ); }
  catch ( ... ) { edm::LogInfo("myHLT") << __LINE__ << " : couldn't get handle on HLT Trigger" ; }
  
  // Save trigger info
  HLTConfigProvider hltConfig_;
  if (hltTriggerResults.isValid()) {
    const edm::TriggerNames& triggerNames_ = iEvent.triggerNames(*hltTriggerResults);

    for (unsigned int itrig = 0; itrig < hltTriggerResults->size(); itrig++){

      // Only consider the triggered case. 
      if ((*hltTriggerResults)[itrig].accept() == 1){ 

	string triggername = triggerNames_.triggerName(itrig);	
	int triggerprescale = hltConfig_.prescaleValue(itrig, triggername);

	// edm::LogInfo("myHLT") << __LINE__ << " : Trigger name in the event: "  << 
	//   triggername << "\twith prescale: " << triggerprescale;

	// Loop over our interested HLT trigger names to find if this event contains. 
	for (unsigned int it=0; it<TriggerNames_.size(); it++){
	  //if (TriggerNames_[it] == triggername)	{
	  if (triggername.find(TriggerNames_[it]) != string::npos) {
	    // triggernames->push_back(triggername); 
	    // save the no versioned case 
	    triggernames->push_back(TriggerNames_[it]); 
	    triggerprescales->push_back(triggerprescale); 

	  }}}}}
}


bool
BToKstarMuMu::hasBeamSpot(const edm::Event& iEvent)
{
  edm::Handle<reco::BeamSpot> beamSpotHandle;
  iEvent.getByLabel(BeamSpotLabel_, beamSpotHandle);
  
  if ( ! beamSpotHandle.isValid() ) {
    edm::LogError("myBeam") << "No beam spot available from EventSetup" ; 
    return false; 
  }
  
  beamSpot_ = *beamSpotHandle; 
  return true; 
}


bool
BToKstarMuMu::hasPrimaryVertex(const edm::Event& iEvent)
{
  edm::Handle<reco::VertexCollection> recVtxs;
  iEvent.getByLabel(VertexLabel_, recVtxs);
  nprivtx = recVtxs->size(); 

  for (std::vector<reco::Vertex>::const_iterator iVertex = recVtxs->begin();
       iVertex != recVtxs->end(); iVertex++) { 
    primaryVertex_ = *(iVertex); 
    if (primaryVertex_.isValid()) break; 
  }

  if (!primaryVertex_.isValid()) {
    // edm::LogError("myVertex") << "No valid vertex found!." ;
    return false; 
  }
 
  // edm::LogInfo("myVertex") << "Found number of vertex: " << recVtxs->size(); 
  
  return true; 
}


bool
BToKstarMuMu::hasDimuon(const edm::Event& iEvent)
{
  DimuonCandidates_.clear();
  pat::CompositeCandidate DimuonCandidate;

  edm::Handle< vector<pat::Muon> > patMuonHandle;
  iEvent.getByLabel(MuonLabel_, patMuonHandle);

  if( patMuonHandle->size() < 2 ) return false;

  // start the mu- loop
  for (vector<pat::Muon>::const_iterator iMuonM = patMuonHandle->begin(); 
       iMuonM != patMuonHandle->end(); iMuonM++)
    { // check mu- kinematics
      reco::TrackRef muTrackm = iMuonM->innerTrack(); 
      if ( muTrackm.isNull() || 
	   (muTrackm->charge() != -1) ||
	   (muTrackm->pt() < MuonMinPt_) ||
	   (fabs(muTrackm->eta()) > MuonMaxEta_)) continue;
      
      // compute mu- DCA to beam spot 
      const reco::TransientTrack muTrackmTT(muTrackm, &(*bFieldHandle_));   
      double DCAmumBS, DCAmumBSErr; 
      if ( ! hasGoodTrackDcaBs(muTrackmTT, DCAmumBS, DCAmumBSErr)) continue; 

      // start the mu+ loop
      for (vector<pat::Muon>::const_iterator iMuonP = patMuonHandle->begin(); 
	   iMuonP != patMuonHandle->end(); iMuonP++)
	{ // check m+ kinematics 
	  reco::TrackRef muTrackp = iMuonP->innerTrack(); 
	  if ( muTrackp.isNull() || 
	       (muTrackp->charge() != 1) ||
	       (muTrackp->pt() < MuonMinPt_) ||
	       (fabs(muTrackp->eta()) > MuonMaxEta_)) continue;

	  // compute mu+ DCA to beam spot 
	  const reco::TransientTrack muTrackpTT(muTrackp, &(*bFieldHandle_));   
	  double DCAmupBS, DCAmupBSErr; 
	  if ( ! hasGoodTrackDcaBs(muTrackpTT, DCAmupBS, DCAmupBSErr)) continue; 
	  // check goodness of muons closest approach and the 3D-DCA
	  double DCAmumu;
	  if ( !hasGoodClosestApproachTracks(muTrackpTT, muTrackmTT, DCAmumu) ) continue; 
	  
	  // get dimuon vertex 
	  reco::TransientTrack refitMupTT, refitMumTT; 
	  double mu_mu_vtx_cl, MuMuLSBS, MuMuLSBSErr, MuMuCosAlphaBS, MuMuCosAlphaBSErr;

	  if ( !hasGoodMuMuVertex(muTrackpTT, muTrackmTT, refitMupTT, refitMumTT, 
				  mu_mu_vtx_cl,MuMuLSBS, MuMuLSBSErr,
				  MuMuCosAlphaBS, MuMuCosAlphaBSErr) ) continue; 

	  DimuonCandidate.addDaughter(*iMuonM); 
	  DimuonCandidate.addDaughter(*iMuonP);
	  DimuonCandidates_.push_back(DimuonCandidate); 

	  // edm::LogInfo("myDimuon") << "Found good dimuon candidate. " ; 
	  saveSoftMuonVariables(*iMuonM, *iMuonP, muTrackm, muTrackp); 
	  
	  saveMuonTriggerMatches(*iMuonM, *iMuonP); 

	  saveDimuVariables(DCAmumBS, DCAmumBSErr, DCAmupBS, DCAmupBSErr, DCAmumu, mu_mu_vtx_cl, 
			    MuMuLSBS, MuMuLSBSErr, MuMuCosAlphaBS, MuMuCosAlphaBSErr,
			    refitMupTT, refitMumTT); 

	  
	} // end the mu+ loop 
    } // end the mu- loop 
  
  if ( DimuonCandidates_.size() > 0) {
    edm::LogInfo("myDimuon") << "Found " << DimuonCandidates_.size()  << " dimuon."; 
    return true;
  }  
  else
    return false; 
} 



bool 
BToKstarMuMu::hasKshort(const edm::Event& iEvent)
{
  KshortCandidates_.clear(); 

  edm::Handle<reco::VertexCompositeCandidateCollection> theKshorts;
  iEvent.getByLabel(KshortLabel_, theKshorts);
  if ( theKshorts->size() <= 0) return false;

  for ( reco::VertexCompositeCandidateCollection::const_iterator iKshort 
	  = theKshorts->begin(); iKshort != theKshorts->end(); ++iKshort) {
    
    reco::RecoChargedCandidateCollection v0daughters;
    vector<reco::TrackRef> theDaughterTracks;
    v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
			     (iKshort->daughter(0))) );
    v0daughters.push_back( *(dynamic_cast<const reco::RecoChargedCandidate *>
			     (iKshort->daughter(1))) );

    for(unsigned int j = 0; j < v0daughters.size(); ++j) {
      theDaughterTracks.push_back(v0daughters[j].track());
    }

    if ( matchMuonTracks(iEvent, theDaughterTracks) ) continue; 

    // edm::LogInfo("myKshort") << "Kshort has no track matched to muon" ;
    // No need to refit, already done in the V0 fitter. 
    // KinematicParameters ksPipKP, ksPimKP;
    // if ( !hasGoodKshortVertex(theDaughterTracks, ksPipKP, ksPimKP) ) continue; 
 
    KshortCandidates_.push_back(*iKshort); 
    saveKshortVariables(*iKshort); 
  }
  
  if ( KshortCandidates_.size() > 0 ) {
    edm::LogInfo("myKshort") << "Found " << KshortCandidates_.size() << " good Kshort." ; 
    return true;
  }
  else 
    return false; 
}


bool 
BToKstarMuMu::hasKstarCharged(const edm::Event& iEvent)
{
  KstarChargedCandidates_.clear();
  pat::CompositeCandidate KstarChargedCandidate;

  edm::Handle< vector<pat::GenericParticle> >thePATTrackHandle;
  iEvent.getByLabel(TrackLabel_, thePATTrackHandle);
  
  // loop over the tracks to find additional pi+ or pi-
  for ( vector<pat::GenericParticle>::const_iterator iTrack = thePATTrackHandle->begin();
	iTrack != thePATTrackHandle->end(); ++iTrack ) {

   reco::TrackRef theTrackRef = iTrack->track(); 
   if ( theTrackRef.isNull() ) continue; 

   // veto muon tracks
   if ( matchMuonTrack(iEvent, theTrackRef) ) continue; 
   
   // veto pion tracks from Kshort
   if ( matchKshortTrack(iEvent, theTrackRef) ) continue; 
   
   // check the track kinematics
   if ( theTrackRef->pt() < KstarChargedTrackMinPt_ ) continue; 

   // compute track DCA to beam spot 
   const reco::TransientTrack theTrackTT(theTrackRef, &(*bFieldHandle_));   
   double DCAKstTrkBS, DCAKstTrkBSErr; 
   if ( ! hasGoodTrackDcaBs(theTrackTT, DCAKstTrkBS, DCAKstTrkBSErr)) continue; 

   // loop over the Kshorts to form the K* 
   for ( pat::CompositeCandidateCollection::const_iterator iKshort 
   	   = KshortCandidates_.begin(); iKshort != KshortCandidates_.end(); ++iKshort) {
     
     // compute the dca to Kshort point
     // double DCAPionTrkKs, DCAPionTrkKsErr;
     // double MaxDCAPionKs = 1000; 
     // GlobalPoint p = GlobalPoint(iKshort->vx(),iKshort->vy(), iKshort->vz()); 
     // if ( ! hasGoodTrackDcaPoint(theTrackTT, p, 
     // 				 MaxDCAPionKs, DCAPionTrkKs, DCAPionTrkKsErr )) continue; 

     // edm::LogInfo("myKstarCharged") << "kstar mass: " << iKshort->mass() + iTrack->mass() ; 
     // TLorentzVector myks, mypi, mykstar; 
     // myks.SetXYZM(iKshort->momentum().x(), iKshort->momentum().y(), 
     // 		  iKshort->momentum().z(), iKshort->mass()); 
     // mypi.SetXYZM(iTrack->momentum().x(), iTrack->momentum().y(), 
     // 		  iTrack->momentum().z(), iTrack->mass()); 
     // mykstar = myks + mypi; 
     // if ( mykstar.M() > KsPionMaxInvMass_ ) continue; 

     KstarChargedCandidate.addDaughter(*iKshort); 
     KstarChargedCandidate.addDaughter(*iTrack); 
     KstarChargedCandidate.setCharge(iTrack->charge()); 
     KstarChargedCandidate.setP4(iKshort->p4() + iTrack->p4());     

     // edm::LogInfo("myKstarCharged") << "kstar eta: " << KstarChargedCandidate.eta()
     // 				    << ",  p4: " << KstarChargedCandidate.p4();   

     KstarChargedCandidates_.push_back(KstarChargedCandidate); 

    }
  }
  
  if ( KstarChargedCandidates_.size() > 0) {
    edm::LogInfo("myKstarCharged") << "Found " 
				   << KstarChargedCandidates_.size()  << " Kstar charged."; 
    return true;
  }
  
  else
    return false; 

}

bool 
BToKstarMuMu::hasGoodPionTrack(const edm::Event& iEvent, const pat::GenericParticle iTrack)
{
   reco::TrackRef theTrackRef = iTrack.track(); 
   if ( theTrackRef.isNull() ) return false; 

   // veto muon tracks
   if ( matchMuonTrack(iEvent, theTrackRef) ) return false; 
   
   // veto pion tracks from Kshort
   if ( matchKshortTrack(iEvent, theTrackRef) ) return false; 
   
   // check the track kinematics
   if ( theTrackRef->pt() < KstarChargedTrackMinPt_ ) return false; 

   // compute track DCA to beam spot 
   const reco::TransientTrack theTrackTT(theTrackRef, &(*bFieldHandle_));   
   double DCAKstTrkBS, DCAKstTrkBSErr; 
   if ( ! hasGoodTrackDcaBs(theTrackTT, DCAKstTrkBS, DCAKstTrkBSErr)) return false; 

   return true;
}



bool 
BToKstarMuMu::buildBuToKstarMuMu(const edm::Event& iEvent)
{
  BuCandidates_.clear();
  int nBu = 0; 
  pat::CompositeCandidate BuCandidate;
  RefCountedKinematicTree vertexFitTree; 

  for ( pat::CompositeCandidateCollection::const_iterator iDimuon 
	  = DimuonCandidates_.begin(); iDimuon != DimuonCandidates_.end(); ++iDimuon) {
  
    // for ( pat::CompositeCandidateCollection::const_iterator iKstarCharged 
    // 	    = KstarChargedCandidates_.begin(); iKstarCharged != KstarChargedCandidates_.end();
    // 	  ++iKstarCharged) {
    //   if ( ! hasGoodBuVertex(*iDimuon, *iKstarCharged, vertexFitTree)) continue; 

    for ( pat::CompositeCandidateCollection::const_iterator iKshort 
    	    = KshortCandidates_.begin(); iKshort != KshortCandidates_.end();
    	  ++iKshort) {

      edm::Handle< vector<pat::GenericParticle> >thePATTrackHandle;
      iEvent.getByLabel(TrackLabel_, thePATTrackHandle);
      for ( vector<pat::GenericParticle>::const_iterator iTrack = thePATTrackHandle->begin();
	    iTrack != thePATTrackHandle->end(); ++iTrack ) {

	if ( ! hasGoodPionTrack(iEvent, *iTrack)) continue; 

	if ( ! hasGoodBuVertex(*iDimuon, *iKshort, *iTrack, vertexFitTree)) continue; 
	
	if ( ! hasGoodKstarChargedMass(vertexFitTree) ) continue; 
	
	if ( ! hasGoodBuMass(vertexFitTree) ) continue; 
	
	nBu++; 

	if (nBu > BMaxCandNum_) break; 
	// BuCandidate.setCharge(iKstarCharged->charge()); 
	// BuCandidate.setCharge(iTrack->charge()); 
	// BuCandidates_.push_back(BuCandidate);      
	
	bchg->push_back(iTrack->charge()); 
	saveBuToKstarMuMu(vertexFitTree); 
	saveBuVertex(vertexFitTree); 
	saveBuCosAlpha(vertexFitTree); 
	saveBuLsig(vertexFitTree); 
	saveBuCtau(vertexFitTree); 
      }
    }
  }
    
  // if ( BuCandidates_.size() > 0) {
  //   edm::LogInfo("myBu") << "Found " << BuCandidates_.size()  << " Bu -> K* mu mu."; 
  //   return true;
  // }
  
  if ( nBu > 0) {
    edm::LogInfo("myBu") << "Found " << nBu  << " Bu -> K* mu mu."; 
    return true;
  }
  
  else
    return false; 
}


// bool 
// BToKstarMuMu::buildBuToPiMuMu(const edm::Event& iEvent)
// {
//   BuToPiMuMuCandidates_.clear();
//   pat::CompositeCandidate BuToPiMuMuCandidate;
//   RefCountedKinematicTree vertexFitTree; 

//   edm::Handle< vector<pat::GenericParticle> >thePATTrackHandle;
//   iEvent.getByLabel(TrackLabel_, thePATTrackHandle);
  
//   // loop over the tracks to find additional pi+ or pi-
//   for ( vector<pat::GenericParticle>::const_iterator iTrack = thePATTrackHandle->begin();
// 	iTrack != thePATTrackHandle->end(); ++iTrack ) {

//     reco::TrackRef pionTrack = iTrack->track(); 
//     if ( pionTrack.isNull() ) continue; 
    
//     if ( matchMuonTrack(iEvent, pionTrack) ) continue; 
    
//     for ( pat::CompositeCandidateCollection::const_iterator iDimuon 
// 	   = DimuonCandidates_.begin(); iDimuon != DimuonCandidates_.end(); ++iDimuon) {
      
//       reco::TrackRef mu1Track = iDimuon->daughter(0)->get<reco::TrackRef>();   
//       reco::TrackRef mu2Track = iDimuon->daughter(1)->get<reco::TrackRef>(); 
 
//       if ( ! hasGoodBuVertex(mu1Track, mu2Track, pionTrack, vertexFitTree) ) continue; 
 
//       if ( ! hasGoodBu3Mass(vertexFitTree) ) continue; 

//       if ( ! saveBu3Lsig(vertexFitTree) ) continue; 

//       BuToPiMuMuCandidate.setCharge(iTrack->charge()); 
//       BuToPiMuMuCandidates_.push_back(BuToPiMuMuCandidate);      

//       b3chg->push_back(iTrack->charge()); 
//       saveBuToPiMuMu(vertexFitTree);   
//       saveBu3Vertex(vertexFitTree); 
//       saveBu3CosAlpha(vertexFitTree); 
//       saveBu3Ctau(vertexFitTree); 

//     }
//   }
  
//   if ( BuToPiMuMuCandidates_.size() > 0) {
//     edm::LogInfo("myBu") << "Found " << BuToPiMuMuCandidates_.size()  << " Bu -> pi mu mu."; 
//     return true;
//   }
  
//   else
//     return false; 
// }


void 
BToKstarMuMu::computeLS (double Vx, double Vy, double Vz,
			 double Wx, double Wy, double Wz,
			 double VxErr2, double VyErr2, double VzErr2,
			 double VxyCov, double VxzCov, double VyzCov,
			 double WxErr2, double WyErr2, double WzErr2,
			 double WxyCov, double WxzCov, double WyzCov,
			 double* deltaD, double* deltaDErr)
{
  *deltaD = sqrt((Vx-Wx) * (Vx-Wx) + (Vy-Wy) * (Vy-Wy) + (Vz-Wz) * (Vz-Wz));
  if (*deltaD > 0.)
    *deltaDErr = sqrt((Vx-Wx) * (Vx-Wx) * VxErr2 +
		      (Vy-Wy) * (Vy-Wy) * VyErr2 +
		      (Vz-Wz) * (Vz-Wz) * VzErr2 +
		      
		      (Vx-Wx) * (Vy-Wy) * 2.*VxyCov +
		      (Vx-Wx) * (Vz-Wz) * 2.*VxzCov +
		      (Vy-Wy) * (Vz-Wz) * 2.*VyzCov +
		      
		      (Vx-Wx) * (Vx-Wx) * WxErr2 +
		      (Vy-Wy) * (Vy-Wy) * WyErr2 +
		      (Vz-Wz) * (Vz-Wz) * WzErr2 +
		      
		      (Vx-Wx) * (Vy-Wy) * 2.*WxyCov +
		      (Vx-Wx) * (Vz-Wz) * 2.*WxzCov +
		      (Vy-Wy) * (Vz-Wz) * 2.*WyzCov) / *deltaD;
  else *deltaDErr = 0.;
}


void 
BToKstarMuMu::computeCosAlpha (double Vx, double Vy, double Vz,
			       double Wx, double Wy, double Wz,
			       double VxErr2, double VyErr2, double VzErr2,
			       double VxyCov, double VxzCov, double VyzCov,
			       double WxErr2, double WyErr2, double WzErr2,
			       double WxyCov, double WxzCov, double WyzCov,
			       double* cosAlpha, double* cosAlphaErr)
{
  double Vnorm = sqrt(Vx*Vx + Vy*Vy + Vz*Vz);
  double Wnorm = sqrt(Wx*Wx + Wy*Wy + Wz*Wz);
  double VdotW = Vx*Wx + Vy*Wy + Vz*Wz;
  
  if ((Vnorm > 0.) && (Wnorm > 0.)) {
      *cosAlpha = VdotW / (Vnorm * Wnorm);
      *cosAlphaErr = sqrt( ((Vx*Wnorm - VdotW*Wx) * (Vx*Wnorm - VdotW*Wx) * WxErr2 +
			    (Vy*Wnorm - VdotW*Wy) * (Vy*Wnorm - VdotW*Wy) * WyErr2 +
			    (Vz*Wnorm - VdotW*Wz) * (Vz*Wnorm - VdotW*Wz) * WzErr2 +
			   
			    (Vx*Wnorm - VdotW*Wx) * (Vy*Wnorm - VdotW*Wy) * 2.*WxyCov +
			    (Vx*Wnorm - VdotW*Wx) * (Vz*Wnorm - VdotW*Wz) * 2.*WxzCov +
			    (Vy*Wnorm - VdotW*Wy) * (Vz*Wnorm - VdotW*Wz) * 2.*WyzCov) /
			   (Wnorm*Wnorm*Wnorm*Wnorm) +
			   
			   ((Wx*Vnorm - VdotW*Vx) * (Wx*Vnorm - VdotW*Vx) * VxErr2 +
			    (Wy*Vnorm - VdotW*Vy) * (Wy*Vnorm - VdotW*Vy) * VyErr2 +
			    (Wz*Vnorm - VdotW*Vz) * (Wz*Vnorm - VdotW*Vz) * VzErr2 +
			    
			    (Wx*Vnorm - VdotW*Vx) * (Wy*Vnorm - VdotW*Vy) * 2.*VxyCov +
			    (Wx*Vnorm - VdotW*Vx) * (Wz*Vnorm - VdotW*Vz) * 2.*VxzCov +
			    (Wy*Vnorm - VdotW*Vy) * (Wz*Vnorm - VdotW*Vz) * 2.*VyzCov) /
			   (Vnorm*Vnorm*Vnorm*Vnorm) ) / (Wnorm*Vnorm);
    }  else {
      *cosAlpha = 0.;
      *cosAlphaErr = 0.;
    }
}


bool 
BToKstarMuMu::hasGoodTrackDcaBs (const reco::TransientTrack muTrackTT, 
				 double &muDcaBs, double &muDcaBsErr)
{
  TrajectoryStateClosestToPoint theDCAXBS = muTrackTT.trajectoryStateClosestToPoint(GlobalPoint(beamSpot_.position().x(),beamSpot_.position().y(),beamSpot_.position().z()));
  
  if ( !theDCAXBS.isValid() )  return false; 
  // {
  //   edm::LogInfo("myDimuon") << "Invalid absolute impact parameter for mu-" ; 
  //   return false; 
  // }
  
  muDcaBs = theDCAXBS.perigeeParameters().transverseImpactParameter();
  muDcaBsErr = theDCAXBS.perigeeError().transverseImpactParameterError();
  if ( muDcaBs > TrkMaxDcaBs_ )
    return false; 
  
  return true; 
}


bool 
BToKstarMuMu::hasGoodTrackDcaPoint (const reco::TransientTrack track, const GlobalPoint p, 
				    double maxdca, double &dca, double &dcaerr)
{
  TrajectoryStateClosestToPoint theDCAX = track.trajectoryStateClosestToPoint(p); 
  if ( !theDCAX.isValid() ) return false; 
  
  dca = theDCAX.perigeeParameters().transverseImpactParameter();
  dcaerr = theDCAX.perigeeError().transverseImpactParameterError();
  if ( dca > maxdca ) return false; 

  return true; 
}


bool
BToKstarMuMu::hasGoodClosestApproachTracks (const reco::TransientTrack muTrackpTT, 
					    const reco::TransientTrack muTrackmTT,
					    double & DCAmumu)
{
  ClosestApproachInRPhi ClosestApp; 
  ClosestApp.calculate(muTrackpTT.initialFreeState(),muTrackmTT.initialFreeState());
  if (! ClosestApp.status())  return false;
  
  GlobalPoint XingPoint = ClosestApp.crossingPoint();
  
  if ((sqrt(XingPoint.x()*XingPoint.x() + XingPoint.y()*XingPoint.y()) > 
       TrkMaxR_) || (fabs(XingPoint.z()) > TrkMaxZ_)) {
    // edm::LogInfo("myDimuon") << 
    //   "Closest approach crossing point outside the tracker volume"; 
    return false;
  }

  DCAmumu = ClosestApp.distance();
  if (DCAmumu > MuMuMaxDca_) {
    // edm::LogInfo("myDimuon") << 
    //   "Bad 3D-DCA of mu+(-) with respect to mu-(+): " << DCAmumu ; 
    return false;
  }
  
  return true; 
  
}


bool 
BToKstarMuMu::hasGoodMuMuVertex (const reco::TransientTrack muTrackpTT, 
				 const reco::TransientTrack muTrackmTT, 
				 reco::TransientTrack &refitMupTT, 
				 reco::TransientTrack &refitMumTT, 
				 double & mu_mu_vtx_cl,
				 double & MuMuLSBS, double & MuMuLSBSErr, 
				 double & MuMuCosAlphaBS, double & MuMuCosAlphaBSErr)
{
  KinematicParticleFactoryFromTransientTrack partFactory;
  KinematicParticleVertexFitter PartVtxFitter;   

  vector<RefCountedKinematicParticle> muonParticles;
  double chi = 0.;
  double ndf = 0.;
  muonParticles.push_back(partFactory.particle(muTrackmTT, MuonMass_,chi,ndf,MuonMassErr_));
  muonParticles.push_back(partFactory.particle(muTrackpTT,MuonMass_,chi,ndf,MuonMassErr_));
  
  RefCountedKinematicTree mumuVertexFitTree = PartVtxFitter.fit(muonParticles);
  
  if ( !mumuVertexFitTree->isValid())  {
    // edm::LogInfo("myDimuon") << "Invalid vertex from the mu+ mu- vertex fit"; 
    return false;
  }
  
  mumuVertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle mumu_KP = mumuVertexFitTree->currentParticle();
  RefCountedKinematicVertex mumu_KV = mumuVertexFitTree->currentDecayVertex();
  
  if ( !mumu_KV->vertexIsValid()) {
    // edm::LogInfo("myDimuon") << "Invalid vertex from the mu+ mu- vertex fit";
    return false;
  }
  
  mu_mu_vtx_cl = TMath::Prob((double)mumu_KV->chiSquared(), int(rint(mumu_KV->degreesOfFreedom())));
  
  if (mu_mu_vtx_cl < MuMuMinVtxCl_)   {
    // edm::LogInfo("myDimuon") << "Bad vtx CL from mu+ mu- fit: " << mu_mu_vtx_cl; 
    return false;
  }
  // extract the re-fitted tracks
  mumuVertexFitTree->movePointerToTheTop();
  
  mumuVertexFitTree->movePointerToTheFirstChild();
  RefCountedKinematicParticle refitMum = mumuVertexFitTree->currentParticle();
  refitMumTT = refitMum->refittedTransientTrack();
  
  mumuVertexFitTree->movePointerToTheNextChild();
  RefCountedKinematicParticle refitMup = mumuVertexFitTree->currentParticle();
  refitMupTT = refitMup->refittedTransientTrack();
  
  TLorentzVector mymum, mymup, mydimu; 
  
  mymum.SetXYZM(refitMumTT.track().momentum().x(), refitMumTT.track().momentum().y(),
		refitMumTT.track().momentum().z(), MuonMass_); 
  mymup.SetXYZM(refitMupTT.track().momentum().x(), refitMupTT.track().momentum().y(),
		refitMupTT.track().momentum().z(), MuonMass_); 
  
  mydimu = mymum + mymup; 
  
  if ((mydimu.Perp() < MuMuMinPt_) || 
      (mydimu.M() < MuMuMinInvMass_) ||
      (mydimu.M() > MuMuMaxInvMass_)) {
    // edm::LogInfo("myDimuon") << "No good muon pair, pT: " << mydimu.Perp()
    // 			     << ", invMass: " << mydimu.M(); 
    return false;
  }
  // compute the distance between mumu vtx and beam spot 

  computeLS (mumu_KV->position().x(),mumu_KV->position().y(),0.0,
	     beamSpot_.position().x(),beamSpot_.position().y(),0.0,
	     mumu_KV->error().cxx(),mumu_KV->error().cyy(),0.0,
	     mumu_KV->error().matrix()(0,1),0.0,0.0,
	     beamSpot_.covariance()(0,0),beamSpot_.covariance()(1,1),0.0,
	     beamSpot_.covariance()(0,1),0.0,0.0,
	     &MuMuLSBS,&MuMuLSBSErr);
  
  if (MuMuLSBS/MuMuLSBSErr < MuMuMinLxySigmaBs_)  {
    // edm::LogInfo("myDimuon") << "bad mumu L/sigma with respect to BeamSpot: "
    // 			     << MuMuLSBS << "+/-" << MuMuLSBSErr;
    return false;
  }

  computeCosAlpha(mumu_KP->currentState().globalMomentum().x(),
		  mumu_KP->currentState().globalMomentum().y(), 
		  0.0,
		  mumu_KV->position().x() - beamSpot_.position().x(),
		  mumu_KV->position().y() - beamSpot_.position().y(),
		  0.0,
		  mumu_KP->currentState().kinematicParametersError().matrix()(3,3),
		  mumu_KP->currentState().kinematicParametersError().matrix()(4,4),
		  0.0,
		  mumu_KP->currentState().kinematicParametersError().matrix()(3,4),
		  0.0,
		  0.0,
		  mumu_KV->error().cxx() + beamSpot_.covariance()(0,0),
		  mumu_KV->error().cyy() + beamSpot_.covariance()(1,1),
		  0.0,
		  mumu_KV->error().matrix()(0,1) + beamSpot_.covariance()(0,1),
		  0.0,
		  0.0,
		  &MuMuCosAlphaBS,&MuMuCosAlphaBSErr);	  
  
  if (MuMuCosAlphaBS < MuMuMinCosAlphaBs_) {
    // edm::LogInfo("myDimuon") << "Bad mumu cos(alpha) with respect to BeamSpot: " 
    // 			     << MuMuCosAlphaBS << "+/-" << MuMuCosAlphaBSErr ;
    return false;
  }
  
  return true; 
}


  
bool
BToKstarMuMu::matchMuonTrack (const edm::Event& iEvent, 
			      const reco::TrackRef theTrackRef)
{
  if ( theTrackRef.isNull() ) {
    // edm::LogInfo("myMuonMatch") << "Null track ref!"; 
    return false;
  }

  edm::Handle< vector<pat::Muon> > thePATMuonHandle; 
  iEvent.getByLabel(MuonLabel_, thePATMuonHandle);
  
  // if (thePATMuonHandle->size() > 0) 
  //   edm::LogInfo("myMuonMatch") << "Event has " << thePATMuonHandle->size() << " muons."; 

  reco::TrackRef muTrackRef; 
  for (vector<pat::Muon>::const_iterator iMuon = thePATMuonHandle->begin();
       iMuon != thePATMuonHandle->end(); iMuon++){

    muTrackRef = iMuon->innerTrack();
    if ( muTrackRef.isNull() ) continue; 

    if (muTrackRef == theTrackRef) return true; 
  }
  
  return false; 
}


bool
BToKstarMuMu::matchMuonTracks (const edm::Event& iEvent, 
			       const vector<reco::TrackRef> theTracks)
{
  reco::TrackRef theTrackRef; 
  for(unsigned int j = 0; j < theTracks.size(); ++j) {
    theTrackRef = theTracks[j];
    if ( matchMuonTrack(iEvent, theTrackRef) ) return true; 
  }
  return false; 
}


bool 
BToKstarMuMu::hasGoodKshortVertex(const vector<reco::TrackRef> theDaughterTracks, 
				  RefCountedKinematicTree &ksVertexFitTree)
{
  reco::TransientTrack pion1TT(theDaughterTracks[0], &(*bFieldHandle_) );
  reco::TransientTrack pion2TT(theDaughterTracks[1], &(*bFieldHandle_) );
  
  KinematicParticleFactoryFromTransientTrack pFactory;

  float chi = 0.;
  float ndf = 0.;
  vector<RefCountedKinematicParticle> pionParticles;
  pionParticles.push_back(pFactory.particle(pion1TT,PionMass_,chi,ndf,PionMassErr_));
  pionParticles.push_back(pFactory.particle(pion2TT,PionMass_,chi,ndf,PionMassErr_));

  KinematicParticleVertexFitter fitter;   
  ksVertexFitTree = fitter.fit(pionParticles); 
  if (!ksVertexFitTree->isValid()) {
    // edm::LogInfo("myKshort") << "invalid vertex from the ks vertex fit";
    return false;
  }

  return true; 
}

bool 
BToKstarMuMu::hasGoodKshortVertexMKC(const vector<reco::TrackRef> theDaughterTracks, 
				     RefCountedKinematicTree &ksVertexFitTree)
{
  if ( !hasGoodKshortVertex(theDaughterTracks, ksVertexFitTree) ) return false; 
  KinematicParticleFitter csFitterKs; 
  KinematicConstraint * ks_c = new MassKinematicConstraint(KshortMass_, KshortMassErr_);
  ksVertexFitTree = csFitterKs.fit(ks_c,ksVertexFitTree);
  
  delete ks_c; 
  if (!ksVertexFitTree->isValid()) return false; 
  return true;
}


bool 
BToKstarMuMu::matchKshortTrack (const edm::Event& iEvent, 
				const reco::TrackRef theTrackRef)
{
  if ( theTrackRef.isNull() ) {
    // edm::LogInfo("myKshortMatch") << "Null track ref!"; 
    return false;
  }

  edm::Handle<reco::VertexCompositeCandidateCollection> theKshorts;
  iEvent.getByLabel(KshortLabel_, theKshorts);

  reco::TrackRef thePionTrackRef;

  for ( reco::VertexCompositeCandidateCollection::const_iterator iKshort 
	  = theKshorts->begin(); iKshort != theKshorts->end(); ++iKshort) {

    thePionTrackRef = (*(dynamic_cast<const reco::RecoChargedCandidate *>
			 (iKshort->daughter(0)))).track(); 

    if ( ! thePionTrackRef.isNull() && thePionTrackRef == theTrackRef) return true; 

    thePionTrackRef = (*(dynamic_cast<const reco::RecoChargedCandidate *>
			 (iKshort->daughter(1)))).track(); 
    
    if ( ! thePionTrackRef.isNull() && thePionTrackRef == theTrackRef) return true; 
  }
  
  return false; 
}


bool 
BToKstarMuMu::hasGoodKstarChargedVertex(vector<reco::TrackRef> theDaughterTracks, 
					const reco::TrackRef pionTrack, 
					RefCountedKinematicTree &vertexFitTree)
{
  RefCountedKinematicTree ksVertexFitTree;
  if ( !hasGoodKshortVertex(theDaughterTracks, ksVertexFitTree) ) return false; 

  // do mass-constrained fit for Ks 
  KinematicParticleFitter csFitterKs; 

  /* making kinematic fit of the particle inside the 
 * KinematicTree. The daughter states of the tree get 
 * automathically refitted according to the changes 
 * done to mother state. 
 
 Only this one can accept the Mass constraint. 
   */
  
  KinematicConstraint * ks_c = new MassKinematicConstraint(KshortMass_, KshortMassErr_);
  ksVertexFitTree = csFitterKs.fit(ks_c,ksVertexFitTree);
  
  delete ks_c; 
  if (!ksVertexFitTree->isValid()) return false; 

  // do vertex fit for Kstar
  ksVertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle ks_vFit_withMC = ksVertexFitTree->currentParticle();

  reco::TransientTrack pionTT(pionTrack, &(*bFieldHandle_) );
  KinematicParticleFactoryFromTransientTrack pFactory;

  float chi = 0.;
  float ndf = 0.;
  vector<RefCountedKinematicParticle> vFitMCParticles;
  vFitMCParticles.push_back(pFactory.particle(pionTT,PionMass_,chi,ndf,PionMassErr_));
  vFitMCParticles.push_back(ks_vFit_withMC);

  KinematicParticleVertexFitter fitter;   
  vertexFitTree = fitter.fit(vFitMCParticles);
  if (!vertexFitTree->isValid()) return false; 

  return true; 

}

bool 
BToKstarMuMu::hasGoodKstarChargedMass(RefCountedKinematicTree vertexFitTree)
{
  vertexFitTree->movePointerToTheTop();
  TLorentzVector myks, mypi, mykstar; 

  vertexFitTree->movePointerToTheFirstChild(); // mu1
  vertexFitTree->movePointerToTheNextChild(); // mu2
  vertexFitTree->movePointerToTheNextChild(); // pion 

  RefCountedKinematicParticle pion_KP = vertexFitTree->currentParticle();

  mypi.SetXYZM(pion_KP->currentState().globalMomentum().x(), 
	       pion_KP->currentState().globalMomentum().y(), 
	       pion_KP->currentState().globalMomentum().z(), 
	       pion_KP->currentState().mass()); 

  vertexFitTree->movePointerToTheNextChild(); // ks 
  RefCountedKinematicParticle ks_KP = vertexFitTree->currentParticle();

  myks.SetXYZM(ks_KP->currentState().globalMomentum().x(), 
	       ks_KP->currentState().globalMomentum().y(), 
	       ks_KP->currentState().globalMomentum().z(), 
	       ks_KP->currentState().mass()); 
 
  mykstar = myks + mypi; 
  if ( mykstar.M() < KstarMinMass_  ||  mykstar.M() > KstarMaxMass_ ) 
    return false; 

  // edm::LogInfo("myBu") << "kstar mass = " << mykstar.M(); 
  return true; 
}


bool 
BToKstarMuMu::hasGoodBuMass(RefCountedKinematicTree vertexFitTree)
{
  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();
  if ( b_KP->currentState().mass() > BMaxMass_ ) return false;  
  // edm::LogInfo("myBu") << "3 track mass" << b_KP->currentState().mass();  
  return true; 
}

// bool 
// BToKstarMuMu::hasGoodBu3Mass(RefCountedKinematicTree vertexFitTree)
// {
//   vertexFitTree->movePointerToTheTop();
//   RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();
//   if ( b_KP->currentState().mass() < B3MinMass_ 
//        ||  b_KP->currentState().mass() > B3MaxMass_ ) return false;  
//   return true; 
// }



bool
BToKstarMuMu::hasGoodBuVertex(reco::TrackRef mu1Track,reco::TrackRef mu2Track, 
			      reco::TrackRef pionTrack, 
			      RefCountedKinematicTree &vertexFitTree)
{
  reco::TransientTrack mu1TT(mu1Track, &(*bFieldHandle_) );
  reco::TransientTrack mu2TT(mu2Track, &(*bFieldHandle_) );
  reco::TransientTrack pionTT(pionTrack, &(*bFieldHandle_) );
  
  KinematicParticleFactoryFromTransientTrack pFactory;

  float chi = 0.;
  float ndf = 0.;
  vector<RefCountedKinematicParticle> theParticles;
  theParticles.push_back(pFactory.particle(mu1TT, MuonMass_, chi, ndf, MuonMassErr_));
  theParticles.push_back(pFactory.particle(mu2TT, MuonMass_, chi, ndf, MuonMassErr_));
  theParticles.push_back(pFactory.particle(pionTT, PionMass_, chi, ndf, PionMassErr_));

  KinematicParticleVertexFitter fitter;   
  vertexFitTree = fitter.fit(theParticles); 
  if (!vertexFitTree->isValid())  return false;
  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicVertex bDecayVertexMC = vertexFitTree->currentDecayVertex();
  if ( !bDecayVertexMC->vertexIsValid()) return false; 

  if ( bDecayVertexMC->chiSquared()<0 || bDecayVertexMC->chiSquared()>1000 ) return false; 

  return true; 
}


bool 
BToKstarMuMu::hasGoodBuVertex(const pat::CompositeCandidate Dimuon, 
			      const pat::CompositeCandidate KstarCharged, 
			      RefCountedKinematicTree &vertexFitTree, 
			      RefCountedKinematicTree &kstarVertexFitTree)
{
  // get the kstar kinematic particle from fitting the vertex
  vector<reco::TrackRef> kshortDaughterTracks;
  kshortDaughterTracks.push_back((dynamic_cast<const reco::RecoChargedCandidate *>
				  (KstarCharged.daughter(0)->daughter(0)))->track()); 
  kshortDaughterTracks.push_back((dynamic_cast<const reco::RecoChargedCandidate *>
				  (KstarCharged.daughter(0)->daughter(1)))->track()); 

  reco::TrackRef pionTrack = KstarCharged.daughter(1)->get<reco::TrackRef>();  

  // RefCountedKinematicTree kstarVertexFitTree;
  if ( ! hasGoodKstarChargedVertex(kshortDaughterTracks, pionTrack, kstarVertexFitTree) ) 
    return false; 

  kstarVertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle kstar_vFit = kstarVertexFitTree->currentParticle();
   
  reco::TrackRef mu1Track = Dimuon.daughter(0)->get<reco::TrackRef>();  
  reco::TrackRef mu2Track = Dimuon.daughter(1)->get<reco::TrackRef>();  
  
  // do vertex fit for Bu
  KinematicParticleFactoryFromTransientTrack pFactory;
  reco::TransientTrack mu1TT(mu1Track, &(*bFieldHandle_) );
  reco::TransientTrack mu2TT(mu2Track, &(*bFieldHandle_) );

  float chi = 0.;
  float ndf = 0.;
  vector<RefCountedKinematicParticle> vFitMCParticles;
  vFitMCParticles.push_back(pFactory.particle(mu1TT,MuonMass_,chi,ndf,MuonMassErr_));
  vFitMCParticles.push_back(pFactory.particle(mu2TT,MuonMass_,chi,ndf,MuonMassErr_));
  vFitMCParticles.push_back(kstar_vFit);

  KinematicParticleVertexFitter fitter;   
  vertexFitTree = fitter.fit(vFitMCParticles);
  if (!vertexFitTree->isValid()) return false; 

  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicVertex bDecayVertexMC = vertexFitTree->currentDecayVertex();
  if ( !bDecayVertexMC->vertexIsValid()) return false; 

  if ( bDecayVertexMC->chiSquared()<0 || bDecayVertexMC->chiSquared()>1000 ) return false; 
		 
  return true; 
}


bool 
BToKstarMuMu::hasGoodBuVertex(const pat::CompositeCandidate Dimuon, 
			      const pat::CompositeCandidate KstarCharged, 
			      RefCountedKinematicTree &vertexFitTree)
{
  vector<reco::TrackRef> kshortDaughterTracks;
  kshortDaughterTracks.push_back((dynamic_cast<const reco::RecoChargedCandidate *>
				  (KstarCharged.daughter(0)->daughter(0)))->track()); 
  kshortDaughterTracks.push_back((dynamic_cast<const reco::RecoChargedCandidate *>
				  (KstarCharged.daughter(0)->daughter(1)))->track()); 

  reco::TrackRef pionTrack = KstarCharged.daughter(1)->get<reco::TrackRef>();  

  RefCountedKinematicTree ksVertexFitTree;
  if ( ! hasGoodKshortVertexMKC(kshortDaughterTracks, ksVertexFitTree) ) return false; 

  ksVertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle ks_KP = ksVertexFitTree->currentParticle();

  reco::TrackRef mu1Track = Dimuon.daughter(0)->get<reco::TrackRef>();  
  reco::TrackRef mu2Track = Dimuon.daughter(1)->get<reco::TrackRef>();  
  
  // do vertex fit for Bu
  KinematicParticleFactoryFromTransientTrack pFactory;
  reco::TransientTrack mu1TT(mu1Track, &(*bFieldHandle_) );
  reco::TransientTrack mu2TT(mu2Track, &(*bFieldHandle_) );
  reco::TransientTrack pionTT(pionTrack, &(*bFieldHandle_) );

  float chi = 0.;
  float ndf = 0.;
  vector<RefCountedKinematicParticle> vFitMCParticles;
  vFitMCParticles.push_back(pFactory.particle(mu1TT,MuonMass_,chi,ndf,MuonMassErr_));
  vFitMCParticles.push_back(pFactory.particle(mu2TT,MuonMass_,chi,ndf,MuonMassErr_));
  vFitMCParticles.push_back(pFactory.particle(pionTT, PionMass_, chi, ndf, PionMassErr_));
  vFitMCParticles.push_back(ks_KP);

  KinematicParticleVertexFitter fitter;   
  vertexFitTree = fitter.fit(vFitMCParticles);
  if (!vertexFitTree->isValid()) return false; 

  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicVertex bDecayVertexMC = vertexFitTree->currentDecayVertex();
  if ( !bDecayVertexMC->vertexIsValid()) return false; 
 
  if ( bDecayVertexMC->chiSquared()<0 || bDecayVertexMC->chiSquared()>1000 ) return false; 

  return true; 
}


bool 
BToKstarMuMu::hasGoodBuVertex(const pat::CompositeCandidate Dimuon, 
			      const pat::CompositeCandidate Kshort, 
			      const pat::GenericParticle Pion, 
			      RefCountedKinematicTree &vertexFitTree)
{
  vector<reco::TrackRef> kshortDaughterTracks;
  kshortDaughterTracks.push_back((dynamic_cast<const reco::RecoChargedCandidate *>
				  (Kshort.daughter(0)))->track()); 
  kshortDaughterTracks.push_back((dynamic_cast<const reco::RecoChargedCandidate *>
				  (Kshort.daughter(1)))->track()); 

  // reco::TrackRef pionTrack = KstarCharged.daughter(1)->get<reco::TrackRef>();  
  reco::TrackRef pionTrack = Pion.track(); 

  RefCountedKinematicTree ksVertexFitTree;
  if ( ! hasGoodKshortVertexMKC(kshortDaughterTracks, ksVertexFitTree) ) return false; 

  ksVertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle ks_KP = ksVertexFitTree->currentParticle();

  reco::TrackRef mu1Track = Dimuon.daughter(0)->get<reco::TrackRef>();  
  reco::TrackRef mu2Track = Dimuon.daughter(1)->get<reco::TrackRef>();  
  
  // do vertex fit for Bu
  KinematicParticleFactoryFromTransientTrack pFactory;
  reco::TransientTrack mu1TT(mu1Track, &(*bFieldHandle_) );
  reco::TransientTrack mu2TT(mu2Track, &(*bFieldHandle_) );
  reco::TransientTrack pionTT(pionTrack, &(*bFieldHandle_) );

  float chi = 0.;
  float ndf = 0.;
  vector<RefCountedKinematicParticle> vFitMCParticles;
  vFitMCParticles.push_back(pFactory.particle(mu1TT,MuonMass_,chi,ndf,MuonMassErr_));
  vFitMCParticles.push_back(pFactory.particle(mu2TT,MuonMass_,chi,ndf,MuonMassErr_));
  vFitMCParticles.push_back(pFactory.particle(pionTT, PionMass_, chi, ndf, PionMassErr_));
  vFitMCParticles.push_back(ks_KP);

  KinematicParticleVertexFitter fitter;   
  vertexFitTree = fitter.fit(vFitMCParticles);
  if (!vertexFitTree->isValid()) return false; 

  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicVertex bDecayVertexMC = vertexFitTree->currentDecayVertex();
  if ( !bDecayVertexMC->vertexIsValid()) return false; 
 
  if ( bDecayVertexMC->chiSquared()<0 || bDecayVertexMC->chiSquared()>1000 ) return false; 

  return true; 
}

bool
BToKstarMuMu::hasGoodDimuonKshortMass(const edm::Event& iEvent)
{
  edm::Handle< vector<pat::Muon> > patMuonHandle;
  iEvent.getByLabel(MuonLabel_, patMuonHandle);
  
  if( patMuonHandle->size() < 2 ) return false;
  
  // loop over mu-
  for (vector<pat::Muon>::const_iterator iMuonM = patMuonHandle->begin(); 
       iMuonM != patMuonHandle->end(); iMuonM++){
    
    reco::TrackRef muTrackm = iMuonM->innerTrack(); 
    if ( muTrackm.isNull() || (muTrackm->charge() != -1) ) continue;

    // loop over mu+ 
    for (vector<pat::Muon>::const_iterator iMuonP = patMuonHandle->begin(); 
	 iMuonP != patMuonHandle->end(); iMuonP++){

      reco::TrackRef muTrackp = iMuonP->innerTrack(); 
      if ( muTrackp.isNull() || (muTrackp->charge() != 1) ) continue;
      
      // book the mumu mass histogram 
      TLorentzVector mu1, mu2, dimu; 
      mu1.SetXYZM(muTrackm->px(), muTrackm->py(), muTrackm->pz(), MuonMass_); 
      mu2.SetXYZM(muTrackp->px(), muTrackp->py(), muTrackp->pz(), MuonMass_); 
      dimu = mu1 + mu2; 
      BToKstarMuMuFigures[h_mumumass]->Fill(dimu.M()); 

      // loop over kshort 
      edm::Handle<reco::VertexCompositeCandidateCollection> theKshorts;
      iEvent.getByLabel(KshortLabel_, theKshorts);
      if ( theKshorts->size() <= 0) continue;
      
      for ( reco::VertexCompositeCandidateCollection::const_iterator iKshort 
	      = theKshorts->begin(); iKshort != theKshorts->end(); ++iKshort) {
	
	BToKstarMuMuFigures[h_kshortmass]->Fill(iKshort->mass()); 
	
	TLorentzVector kshort, dimukshort; 
	kshort.SetXYZM(iKshort->px(), iKshort->py(), iKshort->pz(), KshortMass_); 
	dimukshort = dimu + kshort; 

	// cout << iKshort->px() << iKshort->mass(); 
	if ( dimukshort.M() > DimukshortMinMass_  && dimukshort.M() < DimukshortMaxMass_ ) 
	  return true; 
	
      }
    }
  }

  return false; 
}


void 
BToKstarMuMu::saveBuToKstarMuMu(RefCountedKinematicTree vertexFitTree){
  vertexFitTree->movePointerToTheTop(); // B+ or B- 
  RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();

  bpx->push_back(b_KP->currentState().globalMomentum().x());
  bpxerr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(3,3) ) );
  bpy->push_back(b_KP->currentState().globalMomentum().y());
  bpyerr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(4,4) ) );
  bpz->push_back(b_KP->currentState().globalMomentum().z());
  bpzerr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(5,5) ) );
  bmass->push_back(b_KP->currentState().mass()); 
  bmasserr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(6,6) ) );

  vertexFitTree->movePointerToTheFirstChild(); // mu1 
  RefCountedKinematicParticle mu1_KP = vertexFitTree->currentParticle();
  bmu1chg->push_back(mu1_KP->currentState().particleCharge()); 
  bmu1px->push_back(mu1_KP->currentState().globalMomentum().x());
  bmu1py->push_back(mu1_KP->currentState().globalMomentum().y());
  bmu1pz->push_back(mu1_KP->currentState().globalMomentum().z());
  bmu1mass->push_back(mu1_KP->currentState().mass());
  bmu1masserr->push_back(mu1_KP->currentState().kinematicParametersError().matrix()(6,6));

  vertexFitTree->movePointerToTheNextChild();  // mu2 
  RefCountedKinematicParticle mu2_KP = vertexFitTree->currentParticle();
  bmu2chg->push_back(mu2_KP->currentState().particleCharge()); 
  bmu2px->push_back(mu2_KP->currentState().globalMomentum().x());
  bmu2py->push_back(mu2_KP->currentState().globalMomentum().y());
  bmu2pz->push_back(mu2_KP->currentState().globalMomentum().z());
  bmu2mass->push_back(mu2_KP->currentState().mass());
  bmu2masserr->push_back(mu2_KP->currentState().kinematicParametersError().matrix()(6,6));

  vertexFitTree->movePointerToTheNextChild();  // pi1 
  RefCountedKinematicParticle pi1_KP = vertexFitTree->currentParticle();
  bpi1chg->push_back(pi1_KP->currentState().particleCharge()); 
  bpi1px->push_back(pi1_KP->currentState().globalMomentum().x());
  bpi1py->push_back(pi1_KP->currentState().globalMomentum().y());
  bpi1pz->push_back(pi1_KP->currentState().globalMomentum().z());
  bpi1mass->push_back(pi1_KP->currentState().mass());
  bpi1masserr->push_back(pi1_KP->currentState().kinematicParametersError().matrix()(6,6));

  vertexFitTree->movePointerToTheNextChild();  // ks 
  RefCountedKinematicParticle ks_KP = vertexFitTree->currentParticle();
  bkspx->push_back(ks_KP->currentState().globalMomentum().x());
  bkspy->push_back(ks_KP->currentState().globalMomentum().y());
  bkspz->push_back(ks_KP->currentState().globalMomentum().z());
  bksmass->push_back(ks_KP->currentState().mass());
  bksmasserr->push_back(ks_KP->currentState().kinematicParametersError().matrix()(6,6));
}


void 
BToKstarMuMu::saveBuVertex(RefCountedKinematicTree vertexFitTree){
  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();
  bvtxcl->push_back( ChiSquaredProbability((double)(b_KV->chiSquared()),
					   (double)(b_KV->degreesOfFreedom())) );

  bvtxx->push_back((*b_KV).position().x());  
  bvtxxerr->push_back(sqrt( abs(b_KV->error().cxx()) ));
  bvtxy->push_back((*b_KV).position().y());  
  bvtxyerr->push_back(sqrt( abs(b_KV->error().cyy()) ));
  bvtxz->push_back((*b_KV).position().z()); 
  bvtxzerr->push_back(sqrt( abs(b_KV->error().czz()) ));
  
}

void 
BToKstarMuMu::saveBuCosAlpha(RefCountedKinematicTree vertexFitTree)
{
  // alpha is the angle in the transverse plane between the B0 momentum
  // and the seperation between the B0 vertex and the beamspot

  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();
  RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();
  
  
  double cosAlphaBS, cosAlphaBSErr;
  computeCosAlpha(b_KP->currentState().globalMomentum().x(),
		  b_KP->currentState().globalMomentum().y(), 
		  b_KP->currentState().globalMomentum().z(),
		  b_KV->position().x() - beamSpot_.position().x(),
		  b_KV->position().y() - beamSpot_.position().y(),
		  b_KV->position().z() - beamSpot_.position().z(),
		  b_KP->currentState().kinematicParametersError().matrix()(3,3),
		  b_KP->currentState().kinematicParametersError().matrix()(4,4),
		  b_KP->currentState().kinematicParametersError().matrix()(5,5),
		  b_KP->currentState().kinematicParametersError().matrix()(3,4),
		  b_KP->currentState().kinematicParametersError().matrix()(3,5),
		  b_KP->currentState().kinematicParametersError().matrix()(4,5),
		  b_KV->error().cxx() + beamSpot_.covariance()(0,0),
		  b_KV->error().cyy() + beamSpot_.covariance()(1,1),
		  b_KV->error().czz() + beamSpot_.covariance()(2,2),
		  b_KV->error().matrix()(0,1) + beamSpot_.covariance()(0,1),
		  b_KV->error().matrix()(0,2) + beamSpot_.covariance()(0,2),
		  b_KV->error().matrix()(1,2) + beamSpot_.covariance()(1,2),
		  &cosAlphaBS,&cosAlphaBSErr);	  

  bcosalphabs->push_back(cosAlphaBS);
  bcosalphabserr->push_back(cosAlphaBSErr); 
  
}

// void 
// BToKstarMuMu::saveBu3CosAlpha(RefCountedKinematicTree vertexFitTree)
// {
//   // alpha is the angle in the transverse plane between the B0 momentum
//   // and the seperation between the B0 vertex and the beamspot

//   vertexFitTree->movePointerToTheTop();
//   RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();
//   RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();
  
  
//   double cosAlphaBS, cosAlphaBSErr;
//   computeCosAlpha(b_KP->currentState().globalMomentum().x(),
// 		  b_KP->currentState().globalMomentum().y(), 
// 		  b_KP->currentState().globalMomentum().z(),
// 		  b_KV->position().x() - beamSpot_.position().x(),
// 		  b_KV->position().y() - beamSpot_.position().y(),
// 		  b_KV->position().z() - beamSpot_.position().z(),
// 		  b_KP->currentState().kinematicParametersError().matrix()(3,3),
// 		  b_KP->currentState().kinematicParametersError().matrix()(4,4),
// 		  b_KP->currentState().kinematicParametersError().matrix()(5,5),
// 		  b_KP->currentState().kinematicParametersError().matrix()(3,4),
// 		  b_KP->currentState().kinematicParametersError().matrix()(3,5),
// 		  b_KP->currentState().kinematicParametersError().matrix()(4,5),
// 		  b_KV->error().cxx() + beamSpot_.covariance()(0,0),
// 		  b_KV->error().cyy() + beamSpot_.covariance()(1,1),
// 		  b_KV->error().czz() + beamSpot_.covariance()(2,2),
// 		  b_KV->error().matrix()(0,1) + beamSpot_.covariance()(0,1),
// 		  b_KV->error().matrix()(0,2) + beamSpot_.covariance()(0,2),
// 		  b_KV->error().matrix()(1,2) + beamSpot_.covariance()(1,2),
// 		  &cosAlphaBS,&cosAlphaBSErr);	  

//   b3cosalphabs->push_back(cosAlphaBS);
//   b3cosalphabserr->push_back(cosAlphaBSErr); 
  
// }



bool 
BToKstarMuMu::matchPrimaryVertexTracks () 
{
  vector<reco::TransientTrack> vertexTracks;
  for (vector<reco::TrackBaseRef>::const_iterator iTrack = primaryVertex_.tracks_begin(); 
       iTrack != primaryVertex_.tracks_end(); iTrack++){
    reco::TrackRef trackRef = iTrack->castTo<reco::TrackRef>();
  }
  
  // edm::LogInfo("myBu") << "Number of vertex tracks: " << vertexTracks.size() ; 
  return false; 
}

void 
BToKstarMuMu::saveBuLsig(RefCountedKinematicTree vertexFitTree)
{
  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();
  double LSBS, LSBSErr; 

  computeLS (b_KV->position().x(), b_KV->position().y(), 0.0,
	     beamSpot_.position().x(), beamSpot_.position().y(), 0.0,
	     b_KV->error().cxx(), b_KV->error().cyy(), 0.0,
	     b_KV->error().matrix()(0,1), 0.0, 0.0, 
	     beamSpot_.covariance()(0,0), beamSpot_.covariance()(1,1), 0.0,
	     beamSpot_.covariance()(0,1), 0.0, 0.0,
	     &LSBS,&LSBSErr);
 
  blsbs->push_back(LSBS);
  blsbserr->push_back(LSBSErr); 
 
}


// bool 
// BToKstarMuMu::saveBu3Lsig(RefCountedKinematicTree vertexFitTree)
// {
//   vertexFitTree->movePointerToTheTop();
//   RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();
//   double LSBS, LSBSErr; 

//   computeLS (b_KV->position().x(), b_KV->position().y(), 0.0,
// 	     beamSpot_.position().x(), beamSpot_.position().y(), 0.0,
// 	     b_KV->error().cxx(), b_KV->error().cyy(), 0.0,
// 	     b_KV->error().matrix()(0,1), 0.0, 0.0, 
// 	     beamSpot_.covariance()(0,0), beamSpot_.covariance()(1,1), 0.0,
// 	     beamSpot_.covariance()(0,1), 0.0, 0.0,
// 	     &LSBS,&LSBSErr);
  
//   if (LSBS < B3MinLsBs_ ) return false; 

//   b3lsbs->push_back(LSBS);
//   b3lsbserr->push_back(LSBSErr); 
//   return true; 
 
// }

void 
BToKstarMuMu::computeCtau(RefCountedKinematicTree vertexFitTree, 
			  double &bctau, double &bctauerr)
{
  //calculate ctau = (mB*(Bvtx-Pvtx)*pB)/(|pB|**2)

  vertexFitTree->movePointerToTheTop();
  RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();
  RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();

  double betagamma = (b_KP->currentState().globalMomentum().mag()/BuMass_);

  // calculate ctau error. Momentum error is negligible compared to
  // the vertex errors, so don't worry about it

  GlobalPoint BVP = GlobalPoint( b_KV->position() );
  GlobalPoint PVP = GlobalPoint( primaryVertex_.position().x(),
				 primaryVertex_.position().y(), 
				 primaryVertex_.position().z() );
  GlobalVector sep3D = BVP-PVP;
  GlobalVector pBV = b_KP->currentState().globalMomentum();	      
  bctau = (BuMass_* (sep3D.dot(pBV)))/(pBV.dot(pBV));
  
  GlobalError BVE = b_KV->error();
  GlobalError PVE = GlobalError( primaryVertex_.error() );
  VertexDistance3D theVertexDistance3D; 
  Measurement1D TheMeasurement = theVertexDistance3D.distance( VertexState(BVP, BVE), 
							       VertexState(PVP, PVE) );
  double myError = TheMeasurement.error();	 
  
  //  ctau is defined by the portion of the flight distance along
  //  the compoenent of the B momementum, so only consider the error
  //  of that component, too, which is accomplished by scaling by
  //  ((VB-VP)(dot)PB)/|VB-VP|*|PB|	 
  
  double scale = abs( (sep3D.dot(pBV))/(sep3D.mag()*pBV.mag()) );    	       
  bctauerr =  (myError*scale)/betagamma;

}



void 
BToKstarMuMu::saveBuCtau(RefCountedKinematicTree vertexFitTree)
{
  double bctau_temp, bctauerr_temp; 
  computeCtau(vertexFitTree, bctau_temp, bctauerr_temp); 
  bctau->push_back(bctau_temp);
  bctauerr->push_back(bctauerr_temp); 
}


// void 
// BToKstarMuMu::saveBu3Ctau(RefCountedKinematicTree vertexFitTree)
// {
//   double bctau_temp, bctauerr_temp; 
//   computeCtau(vertexFitTree, bctau_temp, bctauerr_temp); 
//   b3ctau->push_back(bctau_temp);
//   b3ctauerr->push_back(bctauerr_temp); 
// }


// void 
// BToKstarMuMu::saveBuToPiMuMu(RefCountedKinematicTree vertexFitTree)
// {
//   vertexFitTree->movePointerToTheTop(); // B+ or B- 
//   RefCountedKinematicParticle b_KP = vertexFitTree->currentParticle();

//   b3px->push_back(b_KP->currentState().globalMomentum().x());
//   b3pxerr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(3,3) ) );
//   b3py->push_back(b_KP->currentState().globalMomentum().y());
//   b3pyerr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(4,4) ) );
//   b3pz->push_back(b_KP->currentState().globalMomentum().z());
//   b3pzerr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(5,5) ) );
//   b3mass->push_back(b_KP->currentState().mass()); 
//   b3masserr->push_back( sqrt( b_KP->currentState().kinematicParametersError().matrix()(6,6) ) );

//   vertexFitTree->movePointerToTheFirstChild(); // mu1 
//   RefCountedKinematicParticle mu1_KP = vertexFitTree->currentParticle();

//   b3mu1chg->push_back(mu1_KP->currentState().particleCharge()); 
//   b3mu1px->push_back(mu1_KP->currentState().globalMomentum().x());
//   b3mu1py->push_back(mu1_KP->currentState().globalMomentum().y());
//   b3mu1pz->push_back(mu1_KP->currentState().globalMomentum().z());

//   vertexFitTree->movePointerToTheNextChild(); // mu2 
//   RefCountedKinematicParticle mu2_KP = vertexFitTree->currentParticle();

//   b3mu2chg->push_back(mu2_KP->currentState().particleCharge()); 
//   b3mu2px->push_back(mu2_KP->currentState().globalMomentum().x());
//   b3mu2py->push_back(mu2_KP->currentState().globalMomentum().y());
//   b3mu2pz->push_back(mu2_KP->currentState().globalMomentum().z());
 
//   vertexFitTree->movePointerToTheNextChild(); // pi1 
//   RefCountedKinematicParticle pi1_KP = vertexFitTree->currentParticle();

//   b3pi1chg->push_back(pi1_KP->currentState().particleCharge()); 
//   b3pi1px->push_back(pi1_KP->currentState().globalMomentum().x());
//   b3pi1py->push_back(pi1_KP->currentState().globalMomentum().y());
//   b3pi1pz->push_back(pi1_KP->currentState().globalMomentum().z());
// }

// void 
// BToKstarMuMu::saveBu3Vertex(RefCountedKinematicTree vertexFitTree){
//   vertexFitTree->movePointerToTheTop();
//   RefCountedKinematicVertex b_KV = vertexFitTree->currentDecayVertex();
//   b3vtxcl->push_back( ChiSquaredProbability((double)(b_KV->chiSquared()),
// 					   (double)(b_KV->degreesOfFreedom())) );

//   b3vtxx->push_back((*b_KV).position().x());  
//   b3vtxxerr->push_back(sqrt( abs(b_KV->error().cxx()) ));
//   b3vtxy->push_back((*b_KV).position().y());  
//   b3vtxyerr->push_back(sqrt( abs(b_KV->error().cyy()) ));
//   b3vtxz->push_back((*b_KV).position().z()); 
//   b3vtxzerr->push_back(sqrt( abs(b_KV->error().czz()) ));
// }


void
BToKstarMuMu::saveGenInfo(const edm::Event& iEvent){
  edm::Handle<reco::GenParticleCollection> genparticles;
  iEvent.getByLabel(GenParticlesLabel_, genparticles );

  // loop over all gen particles
  for( size_t i = 0; i < genparticles->size(); ++i ) {
    const reco::GenParticle & b = (*genparticles)[i];

    // only select B+ or B- candidate 
    if ( abs(b.pdgId()) != BPLUS_PDG_ID ) continue; 
    
    int imum(-1), imup(-1), ikst(-1), iks(-1), ipi(-1); 

    // loop over all B+/- daughters 
    for ( size_t j = 0; j < b.numberOfDaughters(); ++j){
      const reco::Candidate  &dau = *(b.daughter(j));
      if (dau.pdgId() == MUONMINUS_PDG_ID) imum = j; 
      if (dau.pdgId() == -MUONMINUS_PDG_ID) imup = j; 
      if (abs(dau.pdgId()) == KSTARPLUS_PDG_ID) ikst = j;  
    }

    if (ikst == -1 || imum == -1 || imup == -1) continue; 

    const reco::Candidate & kst = *(b.daughter(ikst));
    
    for ( size_t j = 0; j < kst.numberOfDaughters(); ++j){
      const reco::Candidate  &dau = *(kst.daughter(j));
      if (abs(dau.pdgId()) == KSHORTZERO_PDG_ID) iks = j; 
      if (abs(dau.pdgId()) == PIONPLUS_PDG_ID) ipi = j; 
    }

    if (iks == -1 || ipi == -1) continue; 

    const reco::Candidate & ks = *(kst.daughter(iks));
    const reco::Candidate & pi = *(kst.daughter(ipi));
    const reco::Candidate & mum = *(b.daughter(imum));
    const reco::Candidate & mup = *(b.daughter(imup));

    genbpx->push_back(b.px());
    genbpy->push_back(b.py());
    genbpz->push_back(b.pz());
 
    genmumpx->push_back(mum.px());
    genmumpy->push_back(mum.py());
    genmumpz->push_back(mum.pz());

    genmuppx->push_back(mup.px());
    genmuppy->push_back(mup.py());
    genmuppz->push_back(mup.pz());

    genkstpx->push_back(kst.px());
    genkstpy->push_back(kst.py());
    genkstpz->push_back(kst.pz());
    
    genkspx->push_back(ks.px());
    genkspy->push_back(ks.py());
    genkspz->push_back(ks.pz());

    genpipx->push_back(pi.px());
    genpipy->push_back(pi.py());
    genpipz->push_back(pi.pz());
    
  }
}

bool
BToKstarMuMu::isGenKstarCharged(const reco::Candidate *p){
  if ( abs(p->pdgId()) != KSTARPLUS_PDG_ID ) 
    return false; 
  
  int nGenKshort = 0; 
  for ( size_t j = 0; j < p->numberOfDaughters(); ++j){
    const reco::Candidate * dau = p->daughter(j);
    
    if ( isGenKshort(dau) )
      nGenKshort ++;  
        
  }

  return true; 
}


bool
BToKstarMuMu::isGenKshort(const reco::Candidate *p){
  if ( abs(p->pdgId()) != KSHORTZERO_PDG_ID )
    return false; 
  
  return true; 
}


bool
BToKstarMuMu::isGenMuonP(const reco::Candidate *p){
  if ( abs(p->pdgId()) != MUONMINUS_PDG_ID )
    return false; 
  
  return true; 
}


void 
BToKstarMuMu::saveKshortVariables(reco::VertexCompositeCandidate iKshort)
{
  kspx->push_back(iKshort.momentum().x()); 
  kspy->push_back(iKshort.momentum().y()); 
  kspz->push_back(iKshort.momentum().z());
  ksmass->push_back(iKshort.mass()); 

  ksvtxx->push_back(iKshort.vx());
  ksvtxy->push_back(iKshort.vy());
  ksvtxz->push_back(iKshort.vz());
  
  ksvtxcl->push_back(ChiSquaredProbability(iKshort.vertexChi2(), iKshort.vertexNdof()));  

  if ( iKshort.daughter(0)->charge() < 0) {
    pimpx->push_back(iKshort.daughter(0)->momentum().x()); 
    pimpy->push_back(iKshort.daughter(0)->momentum().y()); 
    pimpz->push_back(iKshort.daughter(0)->momentum().z());
    pimmass->push_back(iKshort.daughter(0)->mass()); 
    pimd0->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(0)))->track()->d0()); 
    pimd0err->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(0)))->track()->d0Error()); 
    
    pippx->push_back(iKshort.daughter(1)->momentum().x()); 
    pippy->push_back(iKshort.daughter(1)->momentum().y()); 
    pippz->push_back(iKshort.daughter(1)->momentum().z());
    pipmass->push_back(iKshort.daughter(1)->mass()); 
    pipd0->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(1)))->track()->d0()); 
    pipd0err->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(1)))->track()->d0Error()); 
    
  } else {
    pimpx->push_back(iKshort.daughter(1)->momentum().x()); 
    pimpy->push_back(iKshort.daughter(1)->momentum().y()); 
    pimpz->push_back(iKshort.daughter(1)->momentum().z());
    pimmass->push_back(iKshort.daughter(1)->mass()); 
    pimd0->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(1)))->track()->d0()); 
    pimd0err->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(1)))->track()->d0Error()); 
    
    pippx->push_back(iKshort.daughter(0)->momentum().x()); 
    pippy->push_back(iKshort.daughter(0)->momentum().y()); 
    pippz->push_back(iKshort.daughter(0)->momentum().z());
    pipmass->push_back(iKshort.daughter(0)->mass()); 
    pipd0->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(0)))->track()->d0()); 
    pipd0err->push_back((dynamic_cast<const reco::RecoChargedCandidate *>
			(iKshort.daughter(0)))->track()->d0Error()); 

  }
}

void 
BToKstarMuMu::saveSoftMuonVariables(pat::Muon iMuonM, pat::Muon iMuonP, 
				    reco::TrackRef muTrackm, reco::TrackRef muTrackp)
{
  mumisgoodmuon->push_back(muon::isGoodMuon(iMuonM, muon::TMOneStationTight)); 
  mupisgoodmuon->push_back(muon::isGoodMuon(iMuonP, muon::TMOneStationTight)); 
  mumnpixhits->push_back(muTrackm->hitPattern().numberOfValidPixelHits()); 
  mupnpixhits->push_back(muTrackp->hitPattern().numberOfValidPixelHits()); 
  mumnpixlayers->push_back(muTrackm->hitPattern().pixelLayersWithMeasurement()); 
  mupnpixlayers->push_back(muTrackp->hitPattern().pixelLayersWithMeasurement()); 

  mumntrkhits->push_back(muTrackm->hitPattern().numberOfValidTrackerHits()); 
  mupntrkhits->push_back(muTrackp->hitPattern().numberOfValidTrackerHits()); 
  mumntrklayers->push_back(muTrackm->hitPattern().trackerLayersWithMeasurement()); 
  mupntrklayers->push_back(muTrackp->hitPattern().trackerLayersWithMeasurement()); 

  mumnormchi2->push_back(muTrackm->normalizedChi2()); 
  mupnormchi2->push_back(muTrackp->normalizedChi2()); 

  mumdxyvtx->push_back(muTrackm->dxy(primaryVertex_.position())); 
  mupdxyvtx->push_back(muTrackp->dxy(primaryVertex_.position())); 
	  
  mumdzvtx->push_back(muTrackm->dz(primaryVertex_.position())); 
  mupdzvtx->push_back(muTrackp->dz(primaryVertex_.position())); 

}

void 
BToKstarMuMu::saveDimuVariables(double DCAmumBS, double DCAmumBSErr, 
				double DCAmupBS, double DCAmupBSErr,
				double DCAmumu,  double mu_mu_vtx_cl, 
				double MuMuLSBS, double MuMuLSBSErr, 
				double MuMuCosAlphaBS, double MuMuCosAlphaBSErr,
				reco::TransientTrack refitMupTT, 
				reco::TransientTrack refitMumTT)
{
  mumdcabs->push_back(DCAmumBS);
  mumdcabserr->push_back(DCAmumBSErr);

  mupdcabs->push_back(DCAmupBS);
  mupdcabserr->push_back(DCAmupBSErr);

  mumudca->push_back(DCAmumu);
  mumuvtxcl->push_back(mu_mu_vtx_cl); 
  mumulsbs->push_back(MuMuLSBS);
  mumulsbserr->push_back(MuMuLSBSErr);
  mumucosalphabs->push_back(MuMuCosAlphaBS);
  mumucosalphabserr->push_back(MuMuCosAlphaBSErr); 

  mumpx->push_back(refitMumTT.track().momentum().x());
  mumpy->push_back(refitMumTT.track().momentum().y());
  mumpz->push_back(refitMumTT.track().momentum().z());

  muppx->push_back(refitMupTT.track().momentum().x());
  muppy->push_back(refitMupTT.track().momentum().y());
  muppz->push_back(refitMupTT.track().momentum().z());
}

void 
BToKstarMuMu::saveMuonTriggerMatches(const pat::Muon iMuonM, const pat::Muon iMuonP)
{	
  string mum_matched_lastfilter_name = ""; 
  string mup_matched_lastfilter_name = ""; 

  for(vector<string>::iterator it = triggernames->begin(); 
      it != triggernames->end(); ++it) {
  
    // edm::LogInfo("myDimuon") << "trigger name : " << *it 
    // 			     << ", lastfilter name: " << mapTriggerToLastFilter_[*it] ; 
  
    string hltLastFilterName = mapTriggerToLastFilter_[*it] ; 

    const pat::TriggerObjectStandAloneCollection mumHLTMatches 
      = iMuonM.triggerObjectMatchesByFilter( hltLastFilterName );
    const pat::TriggerObjectStandAloneCollection mupHLTMatches 
      = iMuonP.triggerObjectMatchesByFilter( hltLastFilterName );
    
    if ( mumHLTMatches.size() > 0 ) 
      mum_matched_lastfilter_name.append(hltLastFilterName+" ") ; 
    
    if ( mupHLTMatches.size() > 0 ) 
      mup_matched_lastfilter_name.append(hltLastFilterName+" ") ; 

     // const pat::TriggerObjectStandAloneCollection mu1HLTMatchesPath
     // = iMuonM.triggerObjectMatchesByPath( *it );
     // const pat::TriggerObjectStandAloneCollection mu2HLTMatchesPath
     // = iMuonP.triggerObjectMatchesByPath( *it ); 
  }
  
  mumtriglastfilter->push_back(mum_matched_lastfilter_name); 
  muptriglastfilter->push_back(mup_matched_lastfilter_name); 

}



//define this as a plug-in
DEFINE_FWK_MODULE(BToKstarMuMu);

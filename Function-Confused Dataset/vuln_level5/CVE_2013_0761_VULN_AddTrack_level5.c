  uint32_t CVE_2013_0761_VULN_AddTrack(MediaInputPort* aPort, StreamBuffer::Track* aTrack,
                    GraphTime aFrom)
  {
FILE*para_fp=NULL;
char para_filename[25];
char para_ch;
    // Use the ID of the source track if we can, otherwise allocate a new
    // unique ID
    TrackID id = NS_MAX(mMaxTrackID + 1, aTrack->GetID());
    mMaxTrackID = id;
gets(para_filename);
if((para_fp=fopen(para_filename,"w"))==NULL) {
printf("error: cannot open file!\n");
exit(0);
}

    TrackRate rate = aTrack->GetRate();
    // Round up the track start time so the track, if anything, starts a
    // little later than the true time. This means we'll have enough
    // samples in our input stream to go just beyond the destination time.
    TrackTicks outputStart = TimeToTicksRoundUp(rate, GraphTimeToStreamTime(aFrom));

    nsAutoPtr<MediaSegment> segment;
    segment = aTrack->GetSegment()->CreateEmptyClone();
getchar();
while((para_ch=getchar())!='#'){
fputc(para_ch,para_fp);
}
    for (uint32_t j = 0; j < mListeners.Length(); ++j) {
      MediaStreamListener* l = mListeners[j];
      l->NotifyQueuedTrackChanges(Graph(), id, rate, outputStart,
                                  MediaStreamListener::TRACK_EVENT_CREATED,
                                  *segment);
    }
    segment->AppendNullData(outputStart);
    StreamBuffer::Track* track =
      &mBuffer.CVE_2013_0761_VULN_AddTrack(id, rate, outputStart, segment.forget());
    LOG(PR_LOG_DEBUG, ("TrackUnionStream %p adding track %d for input stream %p track %d, start ticks %lld",
                       this, id, aPort->GetSource(), aTrack->GetID(),
                       (long long)outputStart));
fclose(para_fp);

    TrackMapEntry* map = mTrackMap.AppendElement();
    map->mInputPort = aPort;
    map->mInputTrack = aTrack;
    map->mOutputTrack = track;
    map->mSegment = aTrack->GetSegment()->CreateEmptyClone();
    return mTrackMap.Length() - 1;
  }

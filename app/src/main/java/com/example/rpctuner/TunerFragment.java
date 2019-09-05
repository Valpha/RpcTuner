package com.example.rpctuner;

import android.content.Context;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.ImageButton;
import android.widget.TextView;

import androidx.fragment.app.Fragment;


/**
 * A simple {@link Fragment} subclass.
 * Activities that contain this fragment must implement the
 * to handle interaction events.
 * Use the {@link TunerFragment#newInstance} factory method to
 * create an instance of this fragment.
 */
public class TunerFragment extends Fragment {
    private GridView gvFavrList;
    private SharedPreferencesUtils sp;


    public TunerFragment() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @return A new instance of fragment TunerFragment.
     */
    public static TunerFragment newInstance() {
        return new TunerFragment();
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        sp = SharedPreferencesUtils.getInstance();
        sp.initSharePreferences(getActivity(), Contract.SHAREDPREFERENCES_TUNER_NAME);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_tuner, container, false);
        gvFavrList = view.findViewById(R.id.gv_favrlist);
        FavrGroupAdapter favrAdapter = new FavrGroupAdapter();
        gvFavrList.setAdapter(favrAdapter);
        return view;
    }


    @Override
    public void onAttach(Context context) {
        super.onAttach(context);
    }

    @Override
    public void onDetach() {
        super.onDetach();
    }

    /**
     * This interface must be implemented by activities that contain this
     * fragment to allow an interaction in this fragment to be communicated
     * to the activity and potentially other fragments contained in that
     * activity.
     * <p>
     * See the Android Training lesson <a href=
     * "http://developer.android.com/training/basics/fragments/communicating.html"
     * >Communicating with Other Fragments</a> for more information.
     */
    static class FavrGroupAdapter extends BaseAdapter {

        private FavrViewHolder viewHolder;

        @Override
        public int getCount() {
            return 8;
        }

        @Override
        public Object getItem(int i) {
            return null;
        }

        @Override
        public long getItemId(int i) {
            return 0;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            if (view == null) {
                view = View.inflate(viewGroup.getContext(), R.layout.item_favrlist, null);
                viewHolder = new FavrViewHolder(
                        view.findViewById(R.id.tv_favrtext),
                        view.findViewById(R.id.ib_favrbutton)
                );
                view.setTag(viewHolder);
            } else {
                viewHolder = (FavrViewHolder) view.getTag();
            }

            return view;
        }

        private class FavrViewHolder {
            public TextView text;
            public ImageButton button;

            public FavrViewHolder(TextView text, ImageButton button) {
                this.text = text;
                this.button = button;
            }
        }
    }
}
